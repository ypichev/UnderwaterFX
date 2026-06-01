#include "PluginProcessor.h"
#include "PluginEditor.h"

// Constructor
UnderwaterFXAudioProcessor::UnderwaterFXAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(juce::AudioProcessor::BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ),
    apvts(*this, nullptr, "PARAMETERS", createParameterLayout())
#else
    : apvts(*this, nullptr, "PARAMETERS", createParameterLayout())
#endif
{
}

// Destructor
UnderwaterFXAudioProcessor::~UnderwaterFXAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout UnderwaterFXAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    // 1. Main Macro FX (0.0 to 1.0)
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ "main_fx", 1 }, "Main Effect",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f
    ));

    // 2. Manual Mode Toggle (0: Auto, 1: Manual)
    layout.add(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID{ "manual_mode", 1 }, "Manual Mode", false
    ));

    // 3. Dive Depth
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ "dive", 1 }, "Dive Depth",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f
    ));

    // 4. Filter Resonance
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ "resonance", 1 }, "Resonance",
        juce::NormalisableRange<float>(0.1f, 5.0f, 0.01f), 0.707f
    ));

    // 5. Dry/Wet Mix
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ "mix", 1 }, "Dry/Wet Mix",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 1.0f
    ));

    // 6. Chorus Mix (Manual Mode)
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ "chorus_mix", 1 }, "Chorus Mix",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f
    ));

    // 7. Reverb Mix (Manual Mode)
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ "reverb_mix", 1 }, "Reverb Mix",
        juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 0.0f
    ));

    return layout;
}

const juce::String UnderwaterFXAudioProcessor::getName() const { return JucePlugin_Name; }
bool UnderwaterFXAudioProcessor::acceptsMidi() const { return false; }
bool UnderwaterFXAudioProcessor::producesMidi() const { return false; }
bool UnderwaterFXAudioProcessor::isMidiEffect() const { return false; }
double UnderwaterFXAudioProcessor::getTailLengthSeconds() const { return 0.0; }
int UnderwaterFXAudioProcessor::getNumPrograms() { return 1; }
int UnderwaterFXAudioProcessor::getCurrentProgram() { return 0; }
void UnderwaterFXAudioProcessor::setCurrentProgram(int index) {}
const juce::String UnderwaterFXAudioProcessor::getProgramName(int index) { return {}; }
void UnderwaterFXAudioProcessor::changeProgramName(int index, const juce::String& newName) {}

void UnderwaterFXAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    // Initialize Filter
    lowPassFilter.prepare(spec);
    lowPassFilter.reset();

    // Initialize Chorus
    chorus.prepare(spec);
    chorus.reset();
    chorus.setRate(0.3f);
    chorus.setDepth(0.2f);
    chorus.setCentreDelay(7.0f);
    chorus.setFeedback(0.2f);

    // Initialize Reverb
    reverb.prepare(spec);
    reverb.reset();
}

void UnderwaterFXAudioProcessor::releaseResources() {}

bool UnderwaterFXAudioProcessor::isBusesLayoutSupported(const juce::AudioProcessor::BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
    return true;
}

void UnderwaterFXAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Store dry signal for Mix blending
    juce::AudioBuffer<float> dryBuffer;
    dryBuffer.makeCopyOf(buffer);

    // --- READ PARAMETERS FROM APVTS ---
    float mainFx = apvts.getRawParameterValue("main_fx")->load();
    bool isManual = apvts.getRawParameterValue("manual_mode")->load() > 0.5f;
    float diveVal = apvts.getRawParameterValue("dive")->load();
    float resVal = apvts.getRawParameterValue("resonance")->load();
    float mixVal = apvts.getRawParameterValue("mix")->load();

    // Read individual effect parameters
    float chorusVal = apvts.getRawParameterValue("chorus_mix")->load();
    float reverbVal = apvts.getRawParameterValue("reverb_mix")->load();

    // Variables for final processing values
    float actualDive = 0.0f;
    float actualRes = 0.707f;
    float actualChorus = 0.0f;
    float actualReverb = 0.0f;

    // --- MODE ROUTING LOGIC ---
    if (isManual)
    {
        // Manual Mode: Read values directly from individual parameters
        actualDive = diveVal;
        actualRes = resVal;
        actualChorus = chorusVal;
        actualReverb = reverbVal;
    }
    else
    {
        // Auto Mode: All parameters are driven by the Main FX macro
        actualDive = mainFx;
        actualRes = juce::jmap(mainFx, 0.0f, 1.0f, 0.707f, 1.5f);
        actualChorus = mainFx * 0.5f; // Cap chorus at 50% in auto mode
        actualReverb = mainFx * 0.5f; // Cap reverb at 50% in auto mode
    }

    // --- APPLY EFFECTS ---

    // 1. Configure Filter Cutoff
    float cutoffFrequency = juce::jmap(actualDive, 0.0f, 1.0f, 20000.0f, 150.0f);
    lowPassFilter.state->setCutOffFrequency(getSampleRate(), cutoffFrequency, actualRes);

    // 2. Configure Chorus and Reverb
    chorus.setMix(actualChorus);

    juce::Reverb::Parameters reverbParams;
    reverbParams.roomSize = 0.7f;
    reverbParams.damping = 0.8f;
    reverbParams.dryLevel = 1.0f;
    reverbParams.wetLevel = actualReverb;
    reverb.setParameters(reverbParams);

    // 3. Create Processing Context
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);

    // 4. Process Audio Chain: CHORUS -> REVERB -> FILTER
    chorus.process(context);
    reverb.process(context);
    lowPassFilter.process(context);

    // --- DRY/WET MIX BLENDING ---
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);
        auto* dryData = dryBuffer.getReadPointer(channel);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            // Blend: Dry Signal * (1 - Mix) + Wet Signal * Mix
            channelData[sample] = dryData[sample] * (1.0f - mixVal) + channelData[sample] * mixVal;
        }
    }

    // CALCULATE RMS LEVELS FOR UI METERS
    float rmsLeft = 0.0f;
    float rmsRight = 0.0f;

    if (buffer.getNumChannels() > 0)
        rmsLeft = buffer.getRMSLevel(0, 0, buffer.getNumSamples());
    if (buffer.getNumChannels() > 1)
        rmsRight = buffer.getRMSLevel(1, 0, buffer.getNumSamples());
    else
        rmsRight = rmsLeft; // Fallback to left channel for mono signals

    leftLevel.store(rmsLeft);
    rightLevel.store(rmsRight);
}

bool UnderwaterFXAudioProcessor::hasEditor() const { return true; }
juce::AudioProcessorEditor* UnderwaterFXAudioProcessor::createEditor() { return new UnderwaterFXAudioProcessorEditor(*this); }

void UnderwaterFXAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void UnderwaterFXAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new UnderwaterFXAudioProcessor();
}
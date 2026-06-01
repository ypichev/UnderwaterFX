#pragma once

#include <JuceHeader.h>
#include <vector>
#include <memory>

class UnderwaterFXAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    UnderwaterFXAudioProcessor();
    ~UnderwaterFXAudioProcessor() override;

    //==============================================================================
    // JUCE AudioProcessor Lifecycle & Processing
    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const juce::AudioProcessor::BusesLayout& layouts) const override;

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    // Plugin UI & Identity
    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    // Program / Preset Management
    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    // State Persistence
    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    //==============================================================================
    // Public UI Metering & Parameter State
    //==============================================================================
    std::atomic<float> leftLevel{ 0.0f };
    std::atomic<float> rightLevel{ 0.0f };

    juce::AudioProcessorValueTreeState apvts;

private:
    //==============================================================================
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    // DSP Engine Components
    juce::dsp::ProcessorDuplicator<juce::dsp::StateVariableFilter::Filter<float>, juce::dsp::StateVariableFilter::Parameters<float>> lowPassFilter;
    juce::dsp::Chorus<float> chorus;
    juce::dsp::Reverb reverb;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UnderwaterFXAudioProcessor)
};
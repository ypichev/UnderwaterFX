#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

// ==============================================================================
// CUSTOM LOOK AND FEEL
// ==============================================================================
class PremiumLookAndFeel : public juce::LookAndFeel_V4
{
public:
    PremiumLookAndFeel();

    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
        float sliderPos, float rotStart, float rotEnd, juce::Slider& slider) override;

    void drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
        bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

    void drawLabel(juce::Graphics& g, juce::Label& label) override;

    // Animation interpolation states
    float animationPulse = 0.0f;
    float transitionAlpha = 0.0f;
};

// ==============================================================================
// AUDIO PROCESSOR EDITOR
// ==============================================================================
class UnderwaterFXAudioProcessorEditor : public juce::AudioProcessorEditor, private juce::Timer
{
public:
    UnderwaterFXAudioProcessorEditor(UnderwaterFXAudioProcessor&);
    ~UnderwaterFXAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics&) override;
    void resized() override;

    // Peak level metering states
    std::atomic<float> leftLevel{ 0.0f };
    std::atomic<float> rightLevel{ 0.0f };

private:
    void timerCallback() override;

    // Context & Theme
    UnderwaterFXAudioProcessor& audioProcessor;
    PremiumLookAndFeel premiumLookAndFeel;

    // Macro & Mode Controls
    juce::Slider mainFxSlider;
    juce::ToggleButton manualModeButton;

    // Sub-Effect Parameter Sliders
    juce::Slider diveSlider;
    juce::Slider resSlider;
    juce::Slider chorusSlider;
    juce::Slider reverbSlider;
    juce::Slider mixSlider;

    // APVTS Attachments Shortcuts
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    std::unique_ptr<Attachment> mainFxAttachment;
    std::unique_ptr<ButtonAttachment> manualModeAttachment;
    std::unique_ptr<Attachment> diveAttachment;
    std::unique_ptr<Attachment> resAttachment;
    std::unique_ptr<Attachment> chorusAttachment;
    std::unique_ptr<Attachment> reverbAttachment;
    std::unique_ptr<Attachment> mixAttachment;

    // UI Animation Phase States
    float pulsePhase = 0.0f;
    float modeTransition = 0.0f;

    // VU Meter Animation Smoothing
    float vuLeft = 0.0f;
    float vuRight = 0.0f;
    juce::Random random;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UnderwaterFXAudioProcessorEditor)
};
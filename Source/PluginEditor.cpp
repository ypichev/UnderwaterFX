#include "PluginProcessor.h"
#include "PluginEditor.h"

// ==============================================================================
// LOOK AND FEEL CUSTOMIZATION
// ==============================================================================
PremiumLookAndFeel::PremiumLookAndFeel()
{
    setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    setColour(juce::Label::textColourId, juce::Colour::fromString("#FFB0B0B0"));
    setColour(juce::TextEditor::textColourId, juce::Colour::fromString("#FF00E5FF"));
    setColour(juce::TextEditor::backgroundColourId, juce::Colour::fromString("#FF050505"));
    setColour(juce::TextEditor::highlightColourId, juce::Colour::fromString("#FFBD00FF").withAlpha(0.4f));
    setColour(juce::TextEditor::focusedOutlineColourId, juce::Colour::fromString("#FF00E5FF"));
    setColour(juce::TextEditor::outlineColourId, juce::Colour::fromString("#FF2A2A2A"));
}

void PremiumLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& label)
{
    auto bounds = label.getLocalBounds().toFloat();
    g.setColour(juce::Colour::fromString("#FF111111"));
    g.fillRoundedRectangle(bounds.reduced(1.0f), 4.0f);
    g.setColour(juce::Colour::fromString("#FF252525"));
    g.drawRoundedRectangle(bounds.reduced(1.0f), 4.0f, 1.0f);

    g.setColour(label.findColour(juce::Label::textColourId));
    g.setFont(juce::Font(12.0f, juce::Font::bold));
    g.drawText(label.getText(), bounds, juce::Justification::centred, false);
}

void PremiumLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
    float sliderPos, float rotStart, float rotEnd, juce::Slider& slider)
{
    auto radius = (float)juce::jmin(width / 2, height / 2) - 6.0f;
    auto centreX = (float)x + (float)width * 0.5f;
    auto centreY = (float)y + (float)height * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotStart + sliderPos * (rotEnd - rotStart);

    auto name = slider.getName();
    juce::Colour colorStart, colorEnd;

    // Assign color gradients based on parameter type
    if (name == "Main" || name == "Mix") {
        colorStart = juce::Colour::fromString("#FF00FF88");
        colorEnd = juce::Colour::fromString("#FF00BFFF");
    }
    else {
        colorStart = juce::Colour::fromString("#FFBD00FF");
        colorEnd = juce::Colour::fromString("#FF00E5FF");
    }

    if (name == "Main" || name == "Mix")
    {
        g.setColour(juce::Colour::fromString("#FF333333"));
        int numTicks = 9;
        for (int i = 0; i < numTicks; ++i)
        {
            auto tAngle = rotStart + (i / (float)(numTicks - 1)) * (rotEnd - rotStart);
            juce::Point<float> tickPos(centreX + (radius + 5.0f) * std::cos(tAngle - juce::MathConstants<float>::halfPi),
                centreY + (radius + 5.0f) * std::sin(tAngle - juce::MathConstants<float>::halfPi));
            g.fillEllipse(tickPos.x - 1.0f, tickPos.y - 1.0f, 2.0f, 2.0f);
        }
    }

    g.setColour(juce::Colour::fromString("#FF111111"));
    g.fillEllipse(rx, ry, rw, rw);
    g.setColour(juce::Colour::fromString("#FF2A2A2A"));
    g.drawEllipse(rx, ry, rw, rw, 1.5f);

    g.setColour(juce::Colour::fromString("#FF0A0A0A"));
    g.fillEllipse(rx + 8.0f, ry + 8.0f, rw - 16.0f, rw - 16.0f);

    juce::Path backgroundArc;
    backgroundArc.addCentredArc(centreX, centreY, radius, radius, 0.0f, rotStart, rotEnd, true);
    g.setColour(juce::Colour::fromString("#FF1E1E1E"));
    g.strokePath(backgroundArc, juce::PathStrokeType(4.5f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    if (sliderPos > 0.0f)
    {
        juce::Path valueArc;
        valueArc.addCentredArc(centreX, centreY, radius, radius, 0.0f, rotStart, angle, true);
        juce::ColourGradient grad(colorStart, centreX, ry, colorEnd, centreX, ry + rw, false);

        g.setColour(colorStart.withAlpha(0.2f));
        g.strokePath(valueArc, juce::PathStrokeType(10.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        g.setGradientFill(grad);
        g.strokePath(valueArc, juce::PathStrokeType(4.5f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    }

    juce::Point<float> thumbPoint(centreX + (radius - 14.0f) * std::cos(angle - juce::MathConstants<float>::halfPi),
        centreY + (radius - 14.0f) * std::sin(angle - juce::MathConstants<float>::halfPi));

    g.setColour(sliderPos > 0.05f ? colorStart.brighter(0.5f) : juce::Colours::grey);
    g.fillEllipse(thumbPoint.x - 3.5f, thumbPoint.y - 3.5f, 7.0f, 7.0f);
}

void PremiumLookAndFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
    bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    auto bounds = button.getLocalBounds().toFloat();
    bool isOn = button.getToggleState();

    if (isOn)
    {
        auto purpleColor = juce::Colour::fromString("#FFBD00FF");

        g.setColour(purpleColor.withAlpha(0.25f));
        g.fillRoundedRectangle(bounds, 15.0f);

        g.setColour(purpleColor.withAlpha(0.9f));
        g.drawRoundedRectangle(bounds.reduced(1.0f), 15.0f, 1.5f);

        g.setColour(juce::Colours::white);
    }
    else
    {
        g.setColour(juce::Colour::fromString("#FF141414"));
        g.fillRoundedRectangle(bounds, 15.0f);

        g.setColour(juce::Colour::fromString("#FF333333"));
        g.drawRoundedRectangle(bounds.reduced(1.0f), 15.0f, 1.0f);

        g.setColour(juce::Colour::fromString("#FF777777"));
    }

    g.setFont(juce::Font(13.0f, juce::Font::bold));
    g.drawText("MANUAL MODE", bounds, juce::Justification::centred, true);
}


// ==============================================================================
// EDITOR CONSTRUCTOR / DESTRUCTOR
// ==============================================================================
UnderwaterFXAudioProcessorEditor::UnderwaterFXAudioProcessorEditor(UnderwaterFXAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(760, 300);
    setLookAndFeel(&premiumLookAndFeel);

    setWantsKeyboardFocus(false);
    setMouseClickGrabsKeyboardFocus(false);

    startTimerHz(30);

    auto setupSlider = [this](juce::Slider& slider, const juce::String& name, const juce::String& paramId) {
        slider.setName(name);
        slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 65, 20);
        slider.setWantsKeyboardFocus(false);
        slider.setMouseClickGrabsKeyboardFocus(false);

        if (auto* param = audioProcessor.apvts.getParameter(paramId)) {
            auto range = param->getNormalisableRange();
            slider.setDoubleClickReturnValue(true, range.convertFrom0to1(param->getDefaultValue()));
        }
        addAndMakeVisible(slider);
        };

    setupSlider(mainFxSlider, "Main", "main_fx");
    setupSlider(diveSlider, "Dive", "dive");
    setupSlider(resSlider, "Reso", "resonance");
    setupSlider(chorusSlider, "Chorus", "chorus_mix");
    setupSlider(reverbSlider, "Reverb", "reverb_mix");
    setupSlider(mixSlider, "Mix", "mix");

    manualModeButton.setWantsKeyboardFocus(false);
    manualModeButton.setMouseClickGrabsKeyboardFocus(false);
    addAndMakeVisible(manualModeButton);

    mainFxAttachment = std::make_unique<Attachment>(audioProcessor.apvts, "main_fx", mainFxSlider);
    manualModeAttachment = std::make_unique<ButtonAttachment>(audioProcessor.apvts, "manual_mode", manualModeButton);
    diveAttachment = std::make_unique<Attachment>(audioProcessor.apvts, "dive", diveSlider);
    resAttachment = std::make_unique<Attachment>(audioProcessor.apvts, "resonance", resSlider);
    chorusAttachment = std::make_unique<Attachment>(audioProcessor.apvts, "chorus_mix", chorusSlider);
    reverbAttachment = std::make_unique<Attachment>(audioProcessor.apvts, "reverb_mix", reverbSlider);
    mixAttachment = std::make_unique<Attachment>(audioProcessor.apvts, "mix", mixSlider);
}

UnderwaterFXAudioProcessorEditor::~UnderwaterFXAudioProcessorEditor()
{
    stopTimer();
    setLookAndFeel(nullptr);
}

// ==============================================================================
// TIMER CALLBACK (ANIMATION & METERING)
// ==============================================================================
void UnderwaterFXAudioProcessorEditor::timerCallback()
{
    pulsePhase += 0.09f;
    if (pulsePhase > juce::MathConstants<float>::twoPi) pulsePhase -= juce::MathConstants<float>::twoPi;
    premiumLookAndFeel.animationPulse = (std::sin(pulsePhase) + 1.0f) * 0.5f;

    if (manualModeButton.getToggleState())
    {
        modeTransition = juce::jmin(1.0f, modeTransition + 0.1f);
    }
    else
    {
        modeTransition = juce::jmax(0.0f, modeTransition - 0.1f);
    }
    premiumLookAndFeel.transitionAlpha = modeTransition;

    float targetLeft = audioProcessor.leftLevel.load() * 4.0f;
    float targetRight = audioProcessor.rightLevel.load() * 4.0f;

    vuLeft = targetLeft > vuLeft ? targetLeft : vuLeft * 0.85f;
    vuRight = targetRight > vuRight ? targetRight : vuRight * 0.85f;

    vuLeft = juce::jlimit(0.0f, 1.0f, vuLeft);
    vuRight = juce::jlimit(0.0f, 1.0f, vuRight);

    repaint();
}

// ==============================================================================
// GRAPHICS RENDER METHODS
// ==============================================================================
void UnderwaterFXAudioProcessorEditor::paint(juce::Graphics& g)
{
    juce::ColourGradient bgGrad(juce::Colour::fromString("#FF1E1E1E"), getWidth() / 2.0f, 0,
        juce::Colour::fromString("#FF090909"), getWidth() / 2.0f, getHeight(), false);
    g.setGradientFill(bgGrad);
    g.fillAll();

    juce::Rectangle<float> panel(170, 20, 420, 250);

    auto dynamicThemeColour = juce::Colour::fromString("#FF00FF88")
        .interpolatedWith(juce::Colour::fromString("#FFBD00FF"), modeTransition);

    // Neon glow background accent lines
    float glowWidth = 10.0f + (premiumLookAndFeel.animationPulse * 5.0f);
    g.setColour(dynamicThemeColour.withAlpha(0.12f + premiumLookAndFeel.animationPulse * 0.08f));

    g.drawLine(95.0f, 140.0f, 170.0f, 140.0f, glowWidth);
    g.drawLine(590.0f, 140.0f, 665.0f, 140.0f, glowWidth);

    g.setColour(dynamicThemeColour.withAlpha(0.8f).interpolatedWith(juce::Colours::white, 0.1f));

    g.drawLine(95.0f, 140.0f, 170.0f, 140.0f, 3.5f);
    g.drawLine(590.0f, 140.0f, 665.0f, 140.0f, 3.5f);

    // Central panel background
    g.setColour(juce::Colour::fromString("#FF0D0D0D"));
    g.fillRoundedRectangle(panel, 18.0f);

    // Central panel dynamic border
    auto activeBorderColour = dynamicThemeColour.withAlpha(0.2f + premiumLookAndFeel.animationPulse * 0.3f);
    auto defaultBorderColour = juce::Colour::fromString("#FF2F2F2F");
    g.setColour(defaultBorderColour.interpolatedWith(activeBorderColour, 0.6f + (0.4f * modeTransition)));
    g.drawRoundedRectangle(panel, 18.0f, 1.5f);

    // Parameter labels
    g.setColour(juce::Colour::fromString("#FF888888"));
    g.setFont(juce::Font(14.0f, juce::Font::bold));
    g.drawText("MAIN FX", 40, 65, 110, 20, juce::Justification::centred);
    g.drawText("DRY / WET", 610, 65, 110, 20, juce::Justification::centred);

    g.setFont(juce::Font(11.0f, juce::Font::bold));
    g.drawText("DIVE", 200, 80, 75, 20, juce::Justification::centred);
    g.drawText("RESO", 295, 80, 75, 20, juce::Justification::centred);
    g.drawText("CHORUS", 390, 80, 75, 20, juce::Justification::centred);
    g.drawText("REVERB", 475, 80, 75, 20, juce::Justification::centred);

    // Stereo VU meter
    int meterX = 240;
    int meterY = 225;
    int meterWidth = 280;
    int ledCount = 24;
    float ledWidth = (meterWidth / (float)ledCount) - 2.0f;

    for (int i = 0; i < ledCount; ++i)
    {
        float ledProgress = i / (float)ledCount;

        juce::Colour ledBaseColour = juce::Colours::green;
        if (ledProgress > 0.65f && ledProgress < 0.85f) ledBaseColour = juce::Colours::orange;
        else if (ledProgress >= 0.85f) ledBaseColour = juce::Colours::red;

        float cx = meterX + i * (ledWidth + 2.0f);

        // Left channel LEDs
        bool isLeftOn = ledProgress <= vuLeft;
        g.setColour(isLeftOn ? ledBaseColour.brighter(0.4f) : ledBaseColour.darker(2.5f).withAlpha(0.2f));
        g.fillRect(cx, (float)meterY, ledWidth, 3.0f);
        if (isLeftOn) {
            g.setColour(ledBaseColour.withAlpha(0.2f));
            g.fillRect(cx - 1.0f, (float)meterY - 1.0f, ledWidth + 2.0f, 5.0f);
        }

        // Right channel LEDs
        bool isRightOn = ledProgress <= vuRight;
        g.setColour(isRightOn ? ledBaseColour.brighter(0.4f) : ledBaseColour.darker(2.5f).withAlpha(0.2f));
        g.fillRect(cx, (float)meterY + 6.0f, ledWidth, 3.0f);
        if (isRightOn) {
            g.setColour(ledBaseColour.withAlpha(0.2f));
            g.fillRect(cx - 1.0f, (float)meterY + 5.0f, ledWidth + 2.0f, 5.0f);
        }
    }

    // Dynamic brand logo
    g.setColour(dynamicThemeColour.withAlpha(0.5f + premiumLookAndFeel.animationPulse * 0.3f));
    g.setFont(juce::Font("Impact", 18.0f, juce::Font::italic));
    g.drawText("UNDERWATER FX", 0, 273, getWidth(), 24, juce::Justification::centred);
}

void UnderwaterFXAudioProcessorEditor::resized()
{
    mainFxSlider.setBounds(40, 95, 110, 110);
    mixSlider.setBounds(610, 95, 110, 110);
    manualModeButton.setBounds(310, 35, 140, 30);

    diveSlider.setBounds(200, 115, 75, 75);
    resSlider.setBounds(295, 115, 75, 75);
    chorusSlider.setBounds(390, 115, 75, 75);
    reverbSlider.setBounds(475, 115, 75, 75);
}
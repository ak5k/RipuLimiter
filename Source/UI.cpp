/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 7.0.12

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "UI.h"

//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
UI::UI(RipuLimiterAudioProcessor& p)
    : audioProcessor(p)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    threshSilder.reset(new juce::Slider("thresh slider"));
    addAndMakeVisible(threshSilder.get());
    threshSilder->setRange(-24, 0, 0.1);
    threshSilder->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    threshSilder->setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    threshSilder->addListener(this);

    threshSilder->setBounds(32, 120, 191, 104);

    gainSlider.reset(new juce::Slider("gain Slider"));
    addAndMakeVisible(gainSlider.get());
    gainSlider->setRange(-24, 0, 0.1);
    gainSlider->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    gainSlider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    gainSlider->addListener(this);

    gainSlider->setBounds(24, 272, 191, 104);

    linkButton.reset(new juce::ToggleButton("link button"));
    addAndMakeVisible(linkButton.get());
    linkButton->setButtonText(TRANS("new button"));
    linkButton->addListener(this);

    linkButton->setBounds(72, 232, 102, 24);

    levelMeter.reset(new LevelMeter());
    addAndMakeVisible(levelMeter.get());
    levelMeter->setName("level meter");

    levelMeter->setBounds(472, 136, 54, 216);

    driveSlider.reset(new juce::Slider("drive Slider"));
    addAndMakeVisible(driveSlider.get());
    driveSlider->setRange(0, 24, 0.1);
    driveSlider->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    driveSlider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    driveSlider->addListener(this);

    driveSlider->setBounds(256, 168, 191, 104);

    oversampleButton.reset(new juce::ToggleButton("oversample button"));
    addAndMakeVisible(oversampleButton.get());
    oversampleButton->setButtonText(TRANS("4x"));
    oversampleButton->addListener(this);

    oversampleButton->setBounds(376, 240, 54, 24);

    kneeSlider.reset(new juce::Slider("knee slider"));
    addAndMakeVisible(kneeSlider.get());
    kneeSlider->setRange(0, 6, 0.1);
    kneeSlider->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    kneeSlider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 40, 20);
    kneeSlider->addListener(this);

    kneeSlider->setBounds(352, 152, 94, 56);

    cascadeButton.reset(new juce::ToggleButton("cascade Button"));
    addAndMakeVisible(cascadeButton.get());
    cascadeButton->setButtonText(juce::CharPointer_UTF8("P\xc3\xb6k\xc3\xa4Leveller"));
    cascadeButton->addListener(this);

    cascadeButton->setBounds(248, 304, 150, 24);

    holdSlider.reset(new juce::Slider("hold slider"));
    addAndMakeVisible(holdSlider.get());
    holdSlider->setRange(1, 20, 0.1);
    holdSlider->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    holdSlider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 40, 20);
    holdSlider->addListener(this);

    holdSlider->setBounds(240, 328, 94, 56);

    releaseSlider.reset(new juce::Slider("release Silder"));
    addAndMakeVisible(releaseSlider.get());
    releaseSlider->setRange(5, 300, 1);
    releaseSlider->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    releaseSlider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 40, 20);
    releaseSlider->addListener(this);

    releaseSlider->setBounds(336, 328, 94, 56);

    deessButton.reset(new juce::ToggleButton("deess Button"));
    addAndMakeVisible(deessButton.get());
    deessButton->setButtonText(TRANS("deEsser"));
    deessButton->addListener(this);

    deessButton->setBounds(368, 304, 79, 24);

    //[UserPreSize]
    threshAttachment.reset(
        new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getAPVTS(), "thresh", *threshSilder)
    );
    gainAttachment.reset(
        new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getAPVTS(), "gain", *gainSlider)
    );
    driveAttachment.reset(
        new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getAPVTS(), "drive", *driveSlider)
    );
    kneeAttachment.reset(
        new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getAPVTS(), "knee", *kneeSlider)
    );
    holdAttachment.reset(
        new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getAPVTS(), "hold", *holdSlider)
    );
    releaseAttachment.reset(
        new juce::AudioProcessorValueTreeState::SliderAttachment(audioProcessor.getAPVTS(), "release", *releaseSlider)
    );

    linkAttachment.reset(
        new juce::AudioProcessorValueTreeState::ButtonAttachment(audioProcessor.getAPVTS(), "link", *linkButton)
    );
    oversampleAttachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(
        audioProcessor.getAPVTS(), "oversample", *oversampleButton
    ));
    cascadeAttachment.reset(
        new juce::AudioProcessorValueTreeState::ButtonAttachment(audioProcessor.getAPVTS(), "cascade", *cascadeButton)
    );
    deEsserAttachment.reset(
        new juce::AudioProcessorValueTreeState::ButtonAttachment(audioProcessor.getAPVTS(), "deEsser", *deessButton)
    );

    //[/UserPreSize]

    setSize(600, 400);

    //[Constructor] You can add your own custom stuff here..
    startTimerHz(60);
    //[/Constructor]
}

UI::~UI()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    threshAttachment = nullptr;
    gainAttachment = nullptr;
    driveAttachment = nullptr;
    kneeAttachment = nullptr;
    holdAttachment = nullptr;
    releaseAttachment = nullptr;

    linkAttachment = nullptr;
    oversampleAttachment = nullptr;
    cascadeAttachment = nullptr;
    deEsserAttachment = nullptr;
    //[/Destructor_pre]

    threshSilder = nullptr;
    gainSlider = nullptr;
    linkButton = nullptr;
    levelMeter = nullptr;
    driveSlider = nullptr;
    oversampleButton = nullptr;
    kneeSlider = nullptr;
    cascadeButton = nullptr;
    holdSlider = nullptr;
    releaseSlider = nullptr;
    deessButton = nullptr;

    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void UI::paint(juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll(juce::Colour(0xff323e44));

    {
        int x = 114, y = 3, width = 370, height = 128;
        juce::String text(TRANS("RipuLimiter"));
        juce::Colour fillColour = juce::Colour(0xffb8b8b8);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.setFont(juce::Font("Comic Sans MS", 94.60f, juce::Font::plain).withTypefaceStyle("Regular"));
        g.drawText(text, x, y, width, height, juce::Justification::centred, true);
    }

    {
        int x = 250, y = 147, width = 206, height = 135;
        juce::Colour strokeColour = juce::Colour(0xffb8b8b8);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(strokeColour);
        g.drawRect(x, y, width, height, 5);
    }

    {
        int x = 258, y = 115, width = 200, height = 30;
        juce::String text(TRANS("Paskialisaattori"));
        juce::Colour fillColour = juce::Colour(0xffb8b8b8);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.setFont(juce::Font("Times New Roman", 25.70f, juce::Font::plain).withTypefaceStyle("Bold Italic"));
        g.drawText(text, x, y, width, height, juce::Justification::centred, true);
    }

    {
        int x = 282, y = 371, width = 26, height = 30;
        juce::String text(TRANS("hold"));
        juce::Colour fillColour = juce::Colour(0xffb8b8b8);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
        g.drawText(text, x, y, width, height, juce::Justification::centred, true);
    }

    {
        int x = 378, y = 371, width = 26, height = 30;
        juce::String text(TRANS("rel"));
        juce::Colour fillColour = juce::Colour(0xffb8b8b8);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.setFont(juce::Font(15.00f, juce::Font::plain).withTypefaceStyle("Regular"));
        g.drawText(text, x, y, width, height, juce::Justification::centred, true);
    }

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void UI::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void UI::sliderValueChanged(juce::Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == threshSilder.get())
    {
        //[UserSliderCode_threshSilder] -- add your slider handling code here..
        if (linkButton->getToggleState())
            gainSlider->setValue(threshSilder->getValue(), juce::NotificationType::sendNotificationAsync);
        //[/UserSliderCode_threshSilder]
    }
    else if (sliderThatWasMoved == gainSlider.get())
    {
        //[UserSliderCode_gainSlider] -- add your slider handling code here..
        if (linkButton->getToggleState())
            threshSilder->setValue(gainSlider->getValue(), juce::NotificationType::sendNotificationAsync);
        //[/UserSliderCode_gainSlider]
    }
    else if (sliderThatWasMoved == driveSlider.get())
    {
        //[UserSliderCode_driveSlider] -- add your slider handling code here..
        //[/UserSliderCode_driveSlider]
    }
    else if (sliderThatWasMoved == kneeSlider.get())
    {
        //[UserSliderCode_kneeSlider] -- add your slider handling code here..
        //[/UserSliderCode_kneeSlider]
    }
    else if (sliderThatWasMoved == holdSlider.get())
    {
        //[UserSliderCode_holdSlider] -- add your slider handling code here..
        //[/UserSliderCode_holdSlider]
    }
    else if (sliderThatWasMoved == releaseSlider.get())
    {
        //[UserSliderCode_releaseSlider] -- add your slider handling code here..
        //[/UserSliderCode_releaseSlider]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void UI::buttonClicked(juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == linkButton.get())
    {
        //[UserButtonCode_linkButton] -- add your button handler code here..
        if (linkButton->getToggleState())
            gainSlider->setValue(threshSilder->getValue(), juce::NotificationType::sendNotificationAsync);
        //[/UserButtonCode_linkButton]
    }
    else if (buttonThatWasClicked == oversampleButton.get())
    {
        //[UserButtonCode_oversampleButton] -- add your button handler code here..
        //[/UserButtonCode_oversampleButton]
    }
    else if (buttonThatWasClicked == cascadeButton.get())
    {
        //[UserButtonCode_cascadeButton] -- add your button handler code here..
        //[/UserButtonCode_cascadeButton]
    }
    else if (buttonThatWasClicked == deessButton.get())
    {
        //[UserButtonCode_deessButton] -- add your button handler code here..
        linkButton->setToggleState(deessButton->getToggleState(), juce::NotificationType::sendNotification);
        if (deessButton->getToggleState())
        {
            holdSlider->setValue(1, juce::NotificationType::sendNotificationAsync);
            releaseSlider->setValue(5, juce::NotificationType::sendNotificationAsync);
        }

        //[/UserButtonCode_deessButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]

//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="UI" componentName="" parentClasses="public juce::Component, public juce::Timer"
                 constructorParams="RipuLimiterAudioProcessor&amp; p" variableInitialisers="audioProcessor(p)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44">
    <TEXT pos="114 3 370 128" fill="solid: ffb8b8b8" hasStroke="0" text="RipuLimiter"
          fontname="Comic Sans MS" fontsize="94.59999999999999" kerning="0.0"
          bold="0" italic="0" justification="36"/>
    <RECT pos="250 147 206 135" fill="solid: ffffff" hasStroke="1" stroke="5, mitered, butt"
          strokeColour="solid: ffb8b8b8"/>
    <TEXT pos="258 115 200 30" fill="solid: ffb8b8b8" hasStroke="0" text="Paskialisaattori"
          fontname="Times New Roman" fontsize="25.7" kerning="0.0" bold="1"
          italic="1" justification="36" typefaceStyle="Bold Italic"/>
    <TEXT pos="282 371 26 30" fill="solid: ffb8b8b8" hasStroke="0" text="hold"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
    <TEXT pos="378 371 26 30" fill="solid: ffb8b8b8" hasStroke="0" text="rel"
          fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
          italic="0" justification="36"/>
  </BACKGROUND>
  <SLIDER name="thresh slider" id="9d3728374906328a" memberName="threshSilder"
          virtualName="" explicitFocusOrder="0" pos="32 120 191 104" min="-24.0"
          max="0.0" int="0.1" style="RotaryHorizontalVerticalDrag" textBoxPos="TextBoxRight"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="gain Slider" id="63e3749118141a0c" memberName="gainSlider"
          virtualName="" explicitFocusOrder="0" pos="24 272 191 104" min="-24.0"
          max="0.0" int="0.1" style="RotaryHorizontalVerticalDrag" textBoxPos="TextBoxRight"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <TOGGLEBUTTON name="link button" id="78a766211d990c5b" memberName="linkButton"
                virtualName="" explicitFocusOrder="0" pos="72 232 102 24" buttonText="new button"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <GENERICCOMPONENT name="level meter" id="45ddf433e27c0af4" memberName="levelMeter"
                    virtualName="" explicitFocusOrder="0" pos="472 136 54 216" class="LevelMeter"
                    params=""/>
  <SLIDER name="drive Slider" id="8dc48c9564766d07" memberName="driveSlider"
          virtualName="" explicitFocusOrder="0" pos="256 168 191 104" min="0.0"
          max="24.0" int="0.1" style="RotaryHorizontalVerticalDrag" textBoxPos="TextBoxRight"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <TOGGLEBUTTON name="oversample button" id="8b6d475b5fb2a294" memberName="oversampleButton"
                virtualName="" explicitFocusOrder="0" pos="376 240 54 24" buttonText="4x"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <SLIDER name="knee slider" id="ec149611c92838fc" memberName="kneeSlider"
          virtualName="" explicitFocusOrder="0" pos="352 152 94 56" min="0.0"
          max="6.0" int="0.1" style="RotaryHorizontalVerticalDrag" textBoxPos="TextBoxRight"
          textBoxEditable="1" textBoxWidth="40" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <TOGGLEBUTTON name="cascade Button" id="3dc2076207369837" memberName="cascadeButton"
                virtualName="" explicitFocusOrder="0" pos="248 304 150 24" buttonText="P&#246;k&#228;Leveller"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <SLIDER name="hold slider" id="dcee75f0aa2b7acb" memberName="holdSlider"
          virtualName="" explicitFocusOrder="0" pos="240 328 94 56" min="1.0"
          max="20.0" int="0.1" style="RotaryHorizontalVerticalDrag" textBoxPos="TextBoxRight"
          textBoxEditable="1" textBoxWidth="40" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="release Silder" id="b345ca1fe849979f" memberName="releaseSlider"
          virtualName="" explicitFocusOrder="0" pos="336 328 94 56" min="5.0"
          max="300.0" int="1.0" style="RotaryHorizontalVerticalDrag" textBoxPos="TextBoxRight"
          textBoxEditable="1" textBoxWidth="40" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <TOGGLEBUTTON name="deess Button" id="415ae62478983f0a" memberName="deessButton"
                virtualName="" explicitFocusOrder="0" pos="368 304 79 24" buttonText="deEsser"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//[EndFile] You can add extra defines here...
//[/EndFile]

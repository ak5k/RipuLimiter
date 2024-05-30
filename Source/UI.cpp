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

    threshSilder->setBounds(72, 144, 191, 104);

    gainSlider.reset(new juce::Slider("gain Slider"));
    addAndMakeVisible(gainSlider.get());
    gainSlider->setRange(-24, 0, 0.1);
    gainSlider->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    gainSlider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    gainSlider->addListener(this);

    gainSlider->setBounds(72, 264, 191, 104);

    linkButton.reset(new juce::ToggleButton("link button"));
    addAndMakeVisible(linkButton.get());
    linkButton->setButtonText(TRANS("new button"));
    linkButton->addListener(this);

    linkButton->setBounds(112, 240, 102, 24);

    levelMeter.reset(new LevelMeter());
    addAndMakeVisible(levelMeter.get());
    levelMeter->setName("level meter");

    levelMeter->setBounds(456, 136, 54, 216);

    driveSlider.reset(new juce::Slider("drive Slider"));
    addAndMakeVisible(driveSlider.get());
    driveSlider->setRange(0, 24, 0.1);
    driveSlider->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    driveSlider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 80, 20);
    driveSlider->addListener(this);

    driveSlider->setBounds(256, 200, 191, 104);

    oversampleButton.reset(new juce::ToggleButton("oversample button"));
    addAndMakeVisible(oversampleButton.get());
    oversampleButton->setButtonText(TRANS("4x"));
    oversampleButton->addListener(this);

    oversampleButton->setBounds(376, 272, 54, 24);

    kneeSlider.reset(new juce::Slider("knee slider"));
    addAndMakeVisible(kneeSlider.get());
    kneeSlider->setRange(0, 6, 0.1);
    kneeSlider->setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    kneeSlider->setTextBoxStyle(juce::Slider::TextBoxRight, false, 40, 20);
    kneeSlider->addListener(this);

    kneeSlider->setBounds(352, 184, 94, 56);

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

    linkAttachment.reset(
        new juce::AudioProcessorValueTreeState::ButtonAttachment(audioProcessor.getAPVTS(), "link", *linkButton)
    );
    oversampleAttachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(
        audioProcessor.getAPVTS(), "oversample", *oversampleButton
    ));
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

    linkAttachment = nullptr;
    oversampleAttachment = nullptr;
    //[/Destructor_pre]

    threshSilder = nullptr;
    gainSlider = nullptr;
    linkButton = nullptr;
    levelMeter = nullptr;
    driveSlider = nullptr;
    oversampleButton = nullptr;
    kneeSlider = nullptr;

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
        int x = 119, y = 11, width = 370, height = 128;
        juce::String text(TRANS("RipuLimiter"));
        juce::Colour fillColour = juce::Colour(0xffb8b8b8);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.setFont(juce::Font("Comic Sans MS", 94.60f, juce::Font::plain).withTypefaceStyle("Regular"));
        g.drawText(text, x, y, width, height, juce::Justification::centred, true);
    }

    {
        int x = 250, y = 179, width = 206, height = 135;
        juce::Colour strokeColour = juce::Colour(0xffb8b8b8);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(strokeColour);
        g.drawRect(x, y, width, height, 5);
    }

    {
        int x = 258, y = 147, width = 200, height = 30;
        juce::String text(TRANS("Paskialisaattori"));
        juce::Colour fillColour = juce::Colour(0xffb8b8b8);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour(fillColour);
        g.setFont(juce::Font("Times New Roman", 25.70f, juce::Font::plain).withTypefaceStyle("Bold Italic"));
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
            gainSlider.get()->setValue(threshSilder.get()->getValue(), juce::NotificationType::sendNotificationAsync);
        //[/UserSliderCode_threshSilder]
    }
    else if (sliderThatWasMoved == gainSlider.get())
    {
        //[UserSliderCode_gainSlider] -- add your slider handling code here..
        if (linkButton->getToggleState())
            threshSilder.get()->setValue(gainSlider.get()->getValue(), juce::NotificationType::sendNotificationAsync);
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
            gainSlider.get()->setValue(threshSilder.get()->getValue(), juce::NotificationType::sendNotificationAsync);
        //[/UserButtonCode_linkButton]
    }
    else if (buttonThatWasClicked == oversampleButton.get())
    {
        //[UserButtonCode_oversampleButton] -- add your button handler code here..
        //[/UserButtonCode_oversampleButton]
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
    <TEXT pos="119 11 370 128" fill="solid: ffb8b8b8" hasStroke="0" text="RipuLimiter"
          fontname="Comic Sans MS" fontsize="94.59999999999999" kerning="0.0"
          bold="0" italic="0" justification="36"/>
    <RECT pos="250 179 206 135" fill="solid: ffffff" hasStroke="1" stroke="5, mitered, butt"
          strokeColour="solid: ffb8b8b8"/>
    <TEXT pos="258 147 200 30" fill="solid: ffb8b8b8" hasStroke="0" text="Paskialisaattori"
          fontname="Times New Roman" fontsize="25.7" kerning="0.0" bold="1"
          italic="1" justification="36" typefaceStyle="Bold Italic"/>
  </BACKGROUND>
  <SLIDER name="thresh slider" id="9d3728374906328a" memberName="threshSilder"
          virtualName="" explicitFocusOrder="0" pos="72 144 191 104" min="-24.0"
          max="0.0" int="0.1" style="RotaryHorizontalVerticalDrag" textBoxPos="TextBoxRight"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <SLIDER name="gain Slider" id="63e3749118141a0c" memberName="gainSlider"
          virtualName="" explicitFocusOrder="0" pos="72 264 191 104" min="-24.0"
          max="0.0" int="0.1" style="RotaryHorizontalVerticalDrag" textBoxPos="TextBoxRight"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <TOGGLEBUTTON name="link button" id="78a766211d990c5b" memberName="linkButton"
                virtualName="" explicitFocusOrder="0" pos="112 240 102 24" buttonText="new button"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <GENERICCOMPONENT name="level meter" id="45ddf433e27c0af4" memberName="levelMeter"
                    virtualName="" explicitFocusOrder="0" pos="456 136 54 216" class="LevelMeter"
                    params=""/>
  <SLIDER name="drive Slider" id="8dc48c9564766d07" memberName="driveSlider"
          virtualName="" explicitFocusOrder="0" pos="256 200 191 104" min="0.0"
          max="24.0" int="0.1" style="RotaryHorizontalVerticalDrag" textBoxPos="TextBoxRight"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
  <TOGGLEBUTTON name="oversample button" id="8b6d475b5fb2a294" memberName="oversampleButton"
                virtualName="" explicitFocusOrder="0" pos="376 272 54 24" buttonText="4x"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
  <SLIDER name="knee slider" id="ec149611c92838fc" memberName="kneeSlider"
          virtualName="" explicitFocusOrder="0" pos="352 184 94 56" min="0.0"
          max="6.0" int="0.1" style="RotaryHorizontalVerticalDrag" textBoxPos="TextBoxRight"
          textBoxEditable="1" textBoxWidth="40" textBoxHeight="20" skewFactor="1.0"
          needsCallback="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif

//[EndFile] You can add extra defines here...
//[/EndFile]

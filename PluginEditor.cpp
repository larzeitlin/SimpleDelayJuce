/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleDelayAudioProcessorEditor::SimpleDelayAudioProcessorEditor (SimpleDelayAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    // params for Wet Sliders
    wet.setSliderStyle(Slider::LinearVertical);
    wet.setRange(0.0, 1.0, 0.001);
    wet.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
    wet.setPopupDisplayEnabled(true, false, false);
    wet.setTextValueSuffix(" Wet Level");
    wet.setValue(0.5);

    addAndMakeVisible(&wet);
    wet.addListener(this);

    // params for Delay Length
    delayLength.setSliderStyle(Slider::LinearVertical);
    delayLength.setRange(10.0, 20000.0, 10.0);
    delayLength.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
    delayLength.setPopupDisplayEnabled(true, false, false);
    delayLength.setTextValueSuffix(" Samples Delay");
    delayLength.setValue(1000.0);

    addAndMakeVisible(&delayLength);
    delayLength.addListener(this);

    // params for Feedback
    feedback.setSliderStyle(Slider::LinearVertical);
    feedback.setRange(0.0, 1.0, 0.001);
    feedback.setTextBoxStyle(Slider::NoTextBox, false, 90, 0);
    feedback.setPopupDisplayEnabled(true, false, false);
    feedback.setTextValueSuffix(" Feedback Level");
    feedback.setValue(0.5);

    addAndMakeVisible(&feedback);
    feedback.addListener(this);
}

SimpleDelayAudioProcessorEditor::~SimpleDelayAudioProcessorEditor()
{
}

//==============================================================================
void SimpleDelayAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Simple Delay", getLocalBounds(), Justification::centred, 1);
}

void SimpleDelayAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    wet.setBounds(40, 30, 20, getHeight() - 60);
    delayLength.setBounds(70, 30, 20, getHeight() - 60);
    feedback.setBounds(100, 30, 20, getHeight() - 60);
}

void SimpleDelayAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    processor.wet = wet.getValue();
    processor.delayLength = delayLength.getValue();
    processor.feedback = feedback.getValue();
}
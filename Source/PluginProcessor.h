/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class _4086AudioProcessor  : public foleys::MagicProcessor
{
public:
    //==============================================================================
    _4086AudioProcessor();
    ~_4086AudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================

    void initialiseBuilder (foleys::MagicGUIBuilder& builder) override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    
    
    
    
    
    
struct Eqsettings
    {
    float LFGainInDecibels{ 0 }, LMFGainInDecibels{ 0 }, HMFGainInDecibels{ 0 }, HFGainInDecibels{ 0 };
    
};

    Eqsettings geteqSettings(juce::AudioProcessorValueTreeState& parameters);
private:
    juce::AudioProcessorValueTreeState parameters; 
    AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    
    
    
    using Filter = juce::dsp::IIR::Filter<float>;


    using MonoChain = juce::dsp::ProcessorChain<Filter, Filter, Filter, Filter>;

    MonoChain leftChain, rightChain;

    enum ChainPositions
    {
        LF,
        LMF,
        HMF,
        HF
    };

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (_4086AudioProcessor)
};

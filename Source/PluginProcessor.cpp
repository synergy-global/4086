/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
_4086AudioProcessor::_4086AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : foleys::MagicProcessor (juce::AudioProcessor::BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), parameters (*this, nullptr, Identifier ("Parameters"), createParameterLayout())

#endif

{
    
    
    
    
    
   FOLEYS_SET_SOURCE_PATH (__FILE__);
    auto file = juce::File::getSpecialLocation (juce::File::currentApplicationFile)
        .getChildFile ("Contents")
        .getChildFile ("Resources")
        .getChildFile ("magic.xml");

    if (file.existsAsFile())
        magicState.setGuiValueTree (file);
    else
    magicState.setGuiValueTree (BinaryData::magic_xml, BinaryData::magic_xmlSize);

}

AudioProcessorValueTreeState::ParameterLayout _4086AudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> params;

    params.push_back (std::make_unique<AudioParameterFloat> ("ingain", "Input Gain", -30.0f, 6.0f, 0.0f));

    params.push_back (std::make_unique<AudioParameterFloat> ("outgain", "Output Gain", -30.0f, 6.0f, 0.0f));
    
    params.push_back (std::make_unique<AudioParameterFloat> ("thresh", "THR", -30.0f, 6.0f, 0.0f));
    params.push_back (std::make_unique<AudioParameterFloat> ("attack", "ATK", -30.0f, 6.0f, 0.0f));
    params.push_back (std::make_unique<AudioParameterFloat> ("release", "REL", -30.0f, 6.0f, 0.0f));
    params.push_back (std::make_unique<AudioParameterFloat> ("ratio", "RAT", -30.0f, 6.0f, 0.0f));
    params.push_back (std::make_unique<AudioParameterFloat> ("LO", "LF", -30.0f, 6.0f, 0.0f));
    params.push_back (std::make_unique<AudioParameterFloat> ("LO-MID", "LMF", -30.0f, 6.0f, 0.0f));
    params.push_back (std::make_unique<AudioParameterFloat> ("HI-MID", "HMF", -30.0f, 6.0f, 0.0f));
    params.push_back (std::make_unique<AudioParameterFloat> ("HI", "HF", -30.0f, 6.0f, 0.0f));
    
    
    
    
    return { params.begin(), params.end() };
}

_4086AudioProcessor::Eqsettings geteqSettings(juce::AudioProcessorValueTreeState& parameters)
{

    _4086AudioProcessor::Eqsettings settings;

    settings.LFGainInDecibels = parameters.getRawParameterValue("LowCut Freq")->load();
    settings.LMFGainInDecibels = parameters.getRawParameterValue("HighCut Freq")->load();
    settings.HMFGainInDecibels = parameters.getRawParameterValue("Peak Freq")->load();
    settings.HFGainInDecibels = parameters.getRawParameterValue("Peak Gain")->load();

   return settings;
}

_4086AudioProcessor::~_4086AudioProcessor()
{
}

//==============================================================================
const juce::String _4086AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool _4086AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool _4086AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool _4086AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double _4086AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int _4086AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int _4086AudioProcessor::getCurrentProgram()
{
    return 0;
}

void _4086AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String _4086AudioProcessor::getProgramName (int index)
{
    return {};
}

void _4086AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void _4086AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void _4086AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool _4086AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void _4086AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...

    }
}

void _4086AudioProcessor::initialiseBuilder (foleys::MagicGUIBuilder& builder)
{
    builder.registerJUCEFactories();

    builder.registerJUCELookAndFeels();
     
}

//==============================================================================
void _4086AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void _4086AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

 
//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new _4086AudioProcessor();
}

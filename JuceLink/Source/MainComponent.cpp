/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"

// include link library.
#include "ableton/Link.hpp"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public AudioAppComponent,
							private Button::Listener,
							private Slider::Listener,
							private Timer
{
public:
    //==============================================================================
    MainContentComponent()
    {
        setSize (800, 600);

        // specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);


		numPeers = 0;

		tempo = 120.0;

		quantum = 4;

		link = new ableton::Link(tempo);
		
		link->enable(false);
		
		link->setNumPeersCallback([this](std::size_t p) {
			numPeers = p;
		});

		link->setTempoCallback([this](const double bpm) {
			tempo = bpm;
		});

		addAndMakeVisible(tempoSlider);
		tempoSlider.setRange(0, 300, 0);
		tempoSlider.setSliderStyle(Slider::LinearBar);
		tempoSlider.setTextBoxStyle(Slider::TextBoxAbove, true, 80, 20);
		tempoSlider.setValue(120);
		tempoSlider.setBoundsRelative(0.3, 0.05, 0.6, 0.1);
		tempoSlider.addListener(this);

		addAndMakeVisible(linkStatus);
		linkStatus.setMultiLine(true);
		linkStatus.setReadOnly(true);
		linkStatus.setCaretVisible(false);
		linkStatus.setColour(TextEditor::backgroundColourId, Colours::white);
		linkStatus.setColour(TextEditor::textColourId, Colours::black);
		linkStatus.setFont(Font(22.0f));
		linkStatus.setBoundsRelative(0.3, 0.3, 0.4, 0.4);

		addAndMakeVisible(linkButton);
		linkButton.setButtonText("Link");
		linkButton.setColour(TextButton::buttonColourId, Colours::grey);
		linkButton.setColour(TextButton::buttonOnColourId, Colours::yellowgreen);
		linkButton.setBoundsRelative(0.05, 0.05, 0.15, 0.1);
		linkButton.addListener(this);

		startTimerHz(60);
		
    }

    ~MainContentComponent()
    {
		link->enable(false);

        shutdownAudio();
    }

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        // This function will be called when the audio device is started, or when
        // its settings (i.e. sample rate, block size, etc) are changed.

        // You can use this function to initialise any resources you might need,
        // but be careful - it will be called on the audio thread, not the GUI thread.

        // For more details, see the help for AudioProcessor::prepareToPlay()
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        // Your audio-processing code goes here!

        // For more details, see the help for AudioProcessor::getNextAudioBlock()

        // Right now we are not producing any data, in which case we need to clear the buffer
        // (to prevent the output of random noise)
        bufferToFill.clearActiveBufferRegion();
    }

    void releaseResources() override
    {
        // This will be called when the audio device stops, or when it is being
        // restarted due to a setting change.

        // For more details, see the help for AudioProcessor::releaseResources()
    }

    //==============================================================================
    void paint (Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

        // You can add your drawing code here!
        String statusInfo = "";

		if (link->isEnabled()) {
			statusInfo += String("Link Enable" + String("\n"));
			statusInfo += String("Callback Tempo: " + String(tempo) + String("\n"));
			statusInfo += String("Callback Peer: " + String(numPeers) + String("\n"));
		}
		else
			statusInfo += String("Link Disable" + String("\n"));

		statusInfo += "-------------------------------\n";

		const auto time = link->clock().micros();
		auto timeline = link->captureAppTimeline();
		statusInfo += genState(time, timeline, link->numPeers(), quantum);

		linkStatus.setText(statusInfo, dontSendNotification);

		tempoSlider.setValue(tempo, dontSendNotification);
    }

    void resized() override
    {
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
        linkStatus.setBoundsRelative(0.3, 0.3, 0.4, 0.4);

		linkButton.setBoundsRelative(0.05, 0.05, 0.15, 0.1);

		tempoSlider.setBoundsRelative(0.3, 0.05, 0.6, 0.1);
    }


    void buttonClicked(Button* button) override
	{
		if (button == &linkButton) {

			linkButton.setToggleState(!linkButton.getToggleState(), dontSendNotification);

			link->enable(linkButton.getToggleState());
		}

	}

	void sliderValueChanged(Slider* slider) override
	{
		if (slider == &tempoSlider) {
		
			auto timeline = link->captureAppTimeline();
			
			timeline.setTempo(tempoSlider.getValue(), link->clock().micros());

			link->commitAudioTimeline(timeline);
		}
	}

    
private:
    //==============================================================================

    // Your private member variables go here...
	TextEditor linkStatus;
	TextButton linkButton;
	Slider tempoSlider;

	ScopedPointer<ableton::Link> link;
	double tempo;
	std::size_t numPeers;
	std::size_t quantum;
	std::chrono::microseconds newValue;

	void timerCallback() override
	{
		repaint();
	}

	void clearLine()
	{
		std::cout << "   \r" << std::flush;
		std::cout.fill(' ');
	}

	String genState(const std::chrono::microseconds time, const ableton::Link::Timeline timeline, const std::size_t numPeers, const double quantum)
	{
		String str = "";
		
		const auto beats = timeline.beatAtTime(time, quantum);
		const auto phase = timeline.phaseAtTime(time, quantum);

		str += String("Peers: " + String(numPeers) + "\n");
		str += String("Quantum: " + String(quantum) + "\n");
		str += String("Tempo: " + String(timeline.tempo()) + "\n");
		str += String("Beats: " + String(beats) + "\n");
		str += String("Phase: " + String(phase) + "\n");

		return str;
	}

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }

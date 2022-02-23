#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "src/main/sequencer/track.hpp"
#include "src/shared/shared.hpp"

enum SequencerMode
{
    Normal,
    Select
};

enum Direction
{
    Up,
    Down,
    Left,
    Right
};

struct Selected
{
    int row{0};
    int col{0};
};

class Sequencer
{
public:
    std::vector<Track> tracks;
    SequencerMode mode{Normal};
    Selected selected;
    bool playing{false};
    int bpm{120};
    int octave{4};
    int numOctaves{8};
    int numSteps{16};
    int curStep{0};
    int curPulse{0};
    int pulsesPerQuarterNote{24};
    int pulsesPer16thNote{pulsesPerQuarterNote / 4};
    int samplesPerPulse{0};
    unsigned prevTransport{0};
    unsigned transport{0};
    unsigned sampleRate{44100};

    Sequencer();
    bool isPlaying();
    void play();
    void stop();
    SequencerMode getMode();
    void setMode(SequencerMode);
    Track& getSelectedTrack();
    Cell& getCell(int row, int col);
    void setCell(int row, int col, Cell cell);
    Cell& getSelectedCell();
    Cell getSelectedCellCopy();
    Selected getSelected();
    void moveSelected(Direction direction);
    SynthSettings& getSynthSettings();
    void toggleCell(int row, int col);
    void selectCell(int row, int col);
    void addTrack();
    int getBpm();
    void setBpm(int newBpm);
    void updateTransport(unsigned newTransport);
    EventMap getEventMap();
};

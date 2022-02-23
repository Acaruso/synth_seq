#include "sequencer.hpp"

#include <iostream>

#include "src/main/util.hpp"

Sequencer::Sequencer()
{
    tracks.push_back(Track());
    setBpm(120);
}

bool Sequencer::isPlaying()
{
    return playing;
}

void Sequencer::play()
{
    playing = true;
}

void Sequencer::stop()
{
    playing = false;
    curStep = 0;
    transport = 0;
    prevTransport = 0;
    curPulse = 0;
}

SequencerMode Sequencer::getMode()
{
    return mode;
}

void Sequencer::setMode(SequencerMode mode_)
{
    mode = mode_;
}

Track& Sequencer::getSelectedTrack()
{
    return tracks[selected.row];
}

Cell& Sequencer::getCell(int row, int col)
{
    return tracks[row].cells[col];
}

void Sequencer::setCell(int row, int col, Cell cell)
{
    tracks[row].cells[col] = cell;
}

Cell& Sequencer::getSelectedCell()
{
    return tracks[selected.row].cells[selected.col];
}

Cell Sequencer::getSelectedCellCopy()
{
    return tracks[selected.row].cells[selected.col];
}

Selected Sequencer::getSelected()
{
    return selected;
}

void Sequencer::moveSelected(Direction direction)
{
    if (direction == Up) {
        selected.row = clamp(selected.row - 1, 0, tracks.size());
    }
    else if (direction == Down) {
        selected.row = clamp(selected.row + 1, 0, tracks.size());
    }
    else if (direction == Left) {
        selected.col = clamp(selected.col - 1, 0, numSteps);
    }
    else if (direction == Right) {
        selected.col = clamp(selected.col + 1, 0, numSteps);
    }
}

SynthSettings& Sequencer::getSynthSettings()
{
    Track& track = getSelectedTrack();
    Cell& cell = getSelectedCell();

    if (mode == Normal) {
        return track.getSynthSettings();
    }
    else {
        if (cell.on) {
            return cell.getSynthSettings();
        }
        else {
            return track.getSynthSettings();
        }
    }
}

void Sequencer::toggleCell(int row, int col)
{
    mode = Select;
    selected = { row, col };

    Cell& cell = getSelectedCell();
    Track& track = getSelectedTrack();

    if (!cell.on) {
        cell.on = true;
        cell.synthSettings = track.getSynthSettings();
    }
    else {
        cell.on = false;
    }
}

void Sequencer::selectCell(int row, int col)
{
    if (
        selected.row == row
        && selected.col == col
        && mode == Select
    ) {
        mode = Normal;
    }
    else {
        mode = Select;
        selected.row = row;
        selected.col = col;
    }
}

void Sequencer::addTrack()
{
    tracks.push_back(Track());
}

int Sequencer::getBpm()
{
    return bpm;
}

void Sequencer::setBpm(int newBpm)
{
    bpm = newBpm;
    double temp = (double)sampleRate / ((double)bpm / (double)60 * (double)pulsesPerQuarterNote);
    samplesPerPulse = temp;
}

void Sequencer::updateTransport(unsigned newTransport)
{
    prevTransport = transport;
    transport = newTransport;
}

EventMap Sequencer::getEventMap()
{
    EventMap eventMap;
    unsigned curSample = 0;

    if (prevTransport == 0) {
        curSample = 0;
    }
    else {
        curSample = prevTransport + (samplesPerPulse - (prevTransport % samplesPerPulse));
    }

    for (; curSample < transport; curSample += samplesPerPulse) {
        curStep = curPulse / pulsesPer16thNote;

        if (curPulse % pulsesPer16thNote == 0) {
            for (int i = 0; i < tracks.size(); i++) {
                Track& track = tracks[i];
                Cell& cell = track.cells[curStep];

                if (cell.on) {
                    Event event(curSample, i, cell.synthSettings);
                    std::string key = makeEventKey(event.sample, event.track);
                    eventMap[key] = event;
                }
            }
        }

        curPulse = (curPulse + 1) % (pulsesPer16thNote * numSteps);
    }

    return eventMap;
}

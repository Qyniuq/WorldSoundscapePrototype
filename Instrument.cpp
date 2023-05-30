#include "Instrument.h"

Instrument::Instrument(std::string folder_dir, int min, int max, std::vector<int> oct, int sleep_val, const ALuint& effectSlot)
    : effectSlot{ effectSlot }, octaves{ oct }, folder_direction{ folder_dir }, min{ min }, max{ max }, sleep_value{ sleep_val }
{
    std::cout << "\nInstrument " << folder_direction << " with " << min << " lowest note, " << max << " highest note and octaves ";
    for (int oct : octaves) { std::cout << oct << ", "; } std::cout << " has been created.";
    sounds = LoadSounds();
}

std::map<int, Sound> Instrument::LoadSounds() {
    std::map<int, Sound> result;
    std::vector<std::string> sharp_notes_names{ "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
    std::vector<std::string> flat_notes_names{ "C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B" };
    int i = min;
    for (int oct : octaves) {
        for (; i <= max;) {
            std::string sharp_name = (i < 0) ? sharp_notes_names[(12 - (-i % 12)) % 12] : sharp_notes_names[i % 12];
            std::string flat_name = (i < 0) ? flat_notes_names[(12 - (-i % 12)) % 12] : flat_notes_names[i % 12];
            result.emplace(std::piecewise_construct, std::forward_as_tuple(i), std::forward_as_tuple(
                (folder_direction + "\\" + sharp_name + std::to_string(oct) + "_var").c_str(), sharp_name, flat_name, i, oct, effectSlot));
            ++i;
            if (sharp_name == "B")
                break;
        }
    }
    return result;
}
#include <chrono>
#include <map>
#include <algorithm>
#include <numeric>
#include <execution>
#include <set>
#include "NoteNames.h"
#include "ScaleLibrary.h"
#include "utilities.h"

bool isAltered(int n) {
    n = getNumberInRange(0, 11, n);
    return (n == 1 || n == 3 || n == 6 || n == 8 || n == 10);
}

struct DynamicNote {
    int value;
    std::string name;
    std::string getName(int n) {
        n = getNumberInRange(-5, 6, n);
        switch (n) {
        case 0:
            return name;
            break;
        case 1:
            return name + "#";
            break;
        case 2:
            return name + "##";
            break;
        case -1:
            return name + "b";
            break;
        case -2:
            return name + "bb";
        default:
            return "case of " + std::to_string(n) + " not contempled in switch function of Note struct";
        }
    }

    DynamicNote(int v, std::string n) : value{ v }, name{ n } {}
    DynamicNote() : DynamicNote(0, "C") {}

    bool operator<(const DynamicNote& other) {
        return value < other.value;
    }

    bool operator!=(const std::string& s) const {
        return name.at(0) != s.at(0);
    }
};

int num_of_alterations(std::vector<int> scale, int index) {
    std::vector<int> natural {-1, 0, 2, 4, 5, 7, 9, 11, 12, 14, 16, 17, 19, 21};
    int root = scale.at(0);
    auto it = natural.begin();
    while (*it < root) {
        ++it;
    }
    it = it + index;
    int first_note = *it;
    while (natural.at(0) != first_note) {
        int temp = natural.at(0) + 12;
        natural.push_back(temp);
        natural.erase(natural.begin());
    }

    natural.erase(natural.begin(), natural.begin() + scale.size());

    int double_alterations{ 0 };
    int normal_alteration{ 0 };
    int i{ 0 };

    for (int n : natural) {
        int dif = getNumberInRange(0, 11, n) - getNumberInRange(0, 11, scale.at(i));
        int difference = std::abs(getNumberInRange(-5, 6, dif));
        if (difference > 1)
            double_alterations += 1;
        else if (difference > 0)
            normal_alteration += 1;
        ++i;
    }

    return normal_alteration + double_alterations * 2;
}

std::vector<std::string> getAlterationsFor7thNoteScale(std::vector<int> scale, bool flat_preference, bool enharmonics, bool double_alterations, bool enharmonics_tonics) {

    int root = scale.at(0);

    std::map<int, std::string> notes;
    notes[0] = "C";
    notes[2] = "D";
    notes[4] = "E";
    notes[5] = "F";
    notes[7] = "G";
    notes[9] = "A";
    notes[11] = "B";

    std::vector<std::string> result;
    auto it = scale.begin();
    bool major = *(it + 2) - *it == 4;

    if (isAltered(*it) || *it == 11 || *it == 4 || *it == 0 || *it == 5) {
        auto flat_or_sharp = num_of_alterations(scale, 0) <=> num_of_alterations(scale, -1);
        auto natural_or_flat = num_of_alterations(scale, 1) <=> num_of_alterations(scale, 0);
        switch (*it) {
        case 0:
            if ((flat_or_sharp > 0 || flat_or_sharp == 0 && !flat_preference) && enharmonics_tonics && enharmonics)
                result.push_back("B#");
            else
                result.push_back("C");
            break;
        case 1:
            if (flat_or_sharp > 0 || flat_or_sharp == 0 && !flat_preference)
                result.push_back("C#");
            else
                result.push_back("Db");
            break;
        case 3:
            if (flat_or_sharp > 0 || flat_or_sharp == 0 && !flat_preference)
                result.push_back("D#");
            else
                result.push_back("Eb");
            break;
        case 4:
            if ((natural_or_flat > 0 || natural_or_flat == 0 && !flat_preference) || !enharmonics_tonics || !enharmonics)
                result.push_back("E");
            else
                result.push_back("Fb");
            break;
        case 5:
            if ((flat_or_sharp > 0 || flat_or_sharp == 0 && !flat_preference) && enharmonics_tonics && enharmonics)
                result.push_back("E#");
            else
                result.push_back("F");
            break;
        case 6:
            if (flat_or_sharp > 0 || flat_or_sharp == 0 && !flat_preference)
                result.push_back("F#");
            else
                result.push_back("Gb");
            break;
        case 8:
            if (flat_or_sharp > 0 || flat_or_sharp == 0 && !flat_preference)
                result.push_back("G#");
            else
                result.push_back("Ab");
            break;
        case 10:

            if (flat_or_sharp > 0 || flat_or_sharp == 0 && !flat_preference)
                result.push_back("A#");
            else
                result.push_back("Bb");
            break;
        case 11:
            if ((natural_or_flat > 0 || natural_or_flat == 0 && !flat_preference) || !enharmonics_tonics || !enharmonics)
                result.push_back("B");
            else
                result.push_back("Cb");
            break;
        }
    }
    else {
        result.push_back(notes[getNumberInRange(0, 11, *it)]);
    }

    std::vector<DynamicNote> dynamic_notes;
    dynamic_notes.emplace_back(DynamicNote(0, "C"));
    dynamic_notes.emplace_back(DynamicNote(2, "D"));
    dynamic_notes.emplace_back(DynamicNote(4, "E"));
    dynamic_notes.emplace_back(DynamicNote(5, "F"));
    dynamic_notes.emplace_back(DynamicNote(7, "G"));
    dynamic_notes.emplace_back(DynamicNote(9, "A"));
    dynamic_notes.emplace_back(DynamicNote(11, "B"));

    while (dynamic_notes.at(0) != result.at(0)) {
        DynamicNote temp = dynamic_notes.at(0);
        dynamic_notes.push_back(temp);
        dynamic_notes.erase(dynamic_notes.begin());
    };

    for (int i{ 1 }; i < scale.size(); ++i) {
        int dif = getNumberInRange(-5, 6, getNumberInRange(0, 11, scale.at(i)) - dynamic_notes.at(i).value);
        if ((dynamic_notes[i].value == 4 || dynamic_notes[i].value == 11) && dif == 1 && !enharmonics) {
            int note_offset = dif;
            result.push_back(dynamic_notes.at(getNumberInRange(0, 6, (i + note_offset))).getName(0));
        }
        else if ((dynamic_notes[i].value == 5 || dynamic_notes[i].value == 0) && dif == -1 && !enharmonics) {
            int note_offset = dif;
            result.push_back(dynamic_notes.at(getNumberInRange(0, 6, (i + note_offset))).getName(0));
        }
        else if ((dif == -2 || dif == 2) && !double_alterations) {
            int note_offset = dif / 2;
            result.push_back(dynamic_notes.at(getNumberInRange(0, 6, (i + note_offset))).getName(0));
        }
        else
            result.push_back(dynamic_notes.at(i).getName(dif));
    }

    return result;
}

std::vector<std::string> getAlterationsForRareScale(std::vector<int>scale) {

    std::multiset<int> natural_notes {-1, 0, 2, 4, 5, 7, 9, 11, 12, 14, 16, 17, 19, 21};
    std::vector<int> final_natural;
    auto minor3rd = std::find(scale.begin(), scale.end(), scale.at(0) + 3);
    bool minor3rd_found{ false };
    if (minor3rd == scale.begin() + 2) {
        minor3rd_found = true;
    }
    int previous_added_note{ -1 };
    scale.push_back(scale.at(0) + 12);
    for (int i{ 0 }; i < scale.size()-1; ++i) {
        int scale_note = getNumberInRange(0, 11, scale.at(i));
        auto nearest_natural_note = std::lower_bound(natural_notes.begin(), natural_notes.end(), scale_note);
        auto previous_natural_note = std::lower_bound(natural_notes.begin(), natural_notes.end(), scale_note);
        --previous_natural_note;
        auto next_nearest_natural_note = std::lower_bound(natural_notes.begin(), natural_notes.end(), (getNumberInRange(0, 11, scale.at(i + 1))));
        if (final_natural.size() != 0 && isAltered(scale_note) && (scale_note - *previous_natural_note) == 1 && final_natural.back() != *previous_natural_note && scale_note != getNumberInRange(0, 11, (scale.at(0) + 10))) {
            final_natural.push_back(*--nearest_natural_note);
        }
        else if (*next_nearest_natural_note == *nearest_natural_note || ((scale_note == 1 || scale_note == 6 || scale_note == 8) && minor3rd_found && i == 0)) {
            final_natural.push_back(*--nearest_natural_note);
        }
        else {
            final_natural.push_back(*nearest_natural_note);
        }
        if (final_natural.size() == 1 && scale.at(1) - scale_note < 3 && scale.at(2) - scale_note > 2) {
            final_natural.push_back(*++nearest_natural_note);
            ++i;
        }
    }
    scale.pop_back();
    std::map<int, DynamicNote> dynamic_notes;
    dynamic_notes[0] = DynamicNote(0, "C");
    dynamic_notes[2] = DynamicNote(2, "D");
    dynamic_notes[4] = DynamicNote(4, "E");
    dynamic_notes[5] = DynamicNote(5, "F");
    dynamic_notes[7] = DynamicNote(7, "G");
    dynamic_notes[9] = DynamicNote(9, "A");
    dynamic_notes[11] = DynamicNote(11, "B");

    int i{ 0 };
    std::vector<std::string> result;
    for (auto n : final_natural) {
        int dif = scale.at(i) - n;
        result.push_back(dynamic_notes[n].getName(dif));
        ++i;
    }

    return result;
}

std::vector<std::string> getScaleNoteNames(std::vector<int> scale, bool flat_preference, bool enharmonics, bool double_alterations, bool enharmonics_tonics)
{
    std::vector<std::string> result;
    if (scale.size() == 7)
        result = getAlterationsFor7thNoteScale(scale, flat_preference, enharmonics, double_alterations, enharmonics_tonics);
    else
        result = getAlterationsForRareScale(scale);

    return result;
}

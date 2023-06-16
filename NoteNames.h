#pragma once
#include <vector>
#include <string>

std::vector<std::string> getScaleNoteNames(std::vector<int> scale, bool flat_preference, bool enharmonics, bool double_alterations, bool enharmonics_on_tonics);
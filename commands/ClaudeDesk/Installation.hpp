#pragma once

#include <string>
#include <vector>

struct Installation {
  std::string name;
  std::string label;
};

std::vector<Installation> FindInstallations();
std::vector<Installation> FindVersionedInstallations();
void        SetActiveInstallation(const std::string &name);
std::string GetActiveDeskSuffix();

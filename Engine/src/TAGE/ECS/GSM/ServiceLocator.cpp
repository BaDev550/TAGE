#include "tagepch.h"
#include "ServiceLocator.h"

std::unordered_map<std::type_index, TAGE::MEM::Ref<void>> TAGE::ServiceLocator::_services;
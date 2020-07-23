/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/stl_util.h"
#include "base/strings/utf_string_conversions.h"
#include "components/country_codes/country_codes.h"

// Pull in definitions for Brave prepopulated engines. It's ugly but these need
// to be built as part of the search_engines static library.
#include "../../../components/search_engines/brave_prepopulated_engines.cc"  // NOLINT
#include "../../../components/search_engines/brave_prepopulated_engines.h"

#define GetDataVersion GetDataVersion_ChromiumImpl
#define GetEngineType GetEngineType_ChromiumImpl
#if defined(OS_ANDROID)
#define GetLocalPrepopulatedEngines GetLocalPrepopulatedEngines_Unused
#endif
#define GetPrepopulatedDefaultSearch GetPrepopulatedDefaultSearch_Unused
#define GetPrepopulatedEngine GetPrepopulatedEngine_Unused
#define GetPrepopulatedEngines GetPrepopulatedEngines_Unused
#include "../../../../components/search_engines/template_url_prepopulate_data.cc"  // NOLINT
#undef GetDataVersion
#undef GetEngineType
#if defined(OS_ANDROID)
#undef GetLocalPrepopulatedEngines
#endif
#undef GetPrepopulatedDefaultSearch
#undef GetPrepopulatedEngine
#undef GetPrepopulatedEngines

namespace TemplateURLPrepopulateData {

namespace {

// Maps BravePrepopulatedEngineID to Chromium's PrepopulatedEngine.
const std::map<BravePrepopulatedEngineID, const PrepopulatedEngine*>
    brave_engines_map = {
        {PREPOPULATED_ENGINE_ID_GOOGLE, &google},
        {PREPOPULATED_ENGINE_ID_YAHOO, &brave_yahoo},
        {PREPOPULATED_ENGINE_ID_YAHOO_AR, &brave_yahoo_ar},
        {PREPOPULATED_ENGINE_ID_YAHOO_AT, &brave_yahoo_at},
        {PREPOPULATED_ENGINE_ID_YAHOO_AU, &brave_yahoo_au},
        {PREPOPULATED_ENGINE_ID_YAHOO_BR, &brave_yahoo_br},
        {PREPOPULATED_ENGINE_ID_YAHOO_CA, &brave_yahoo_ca},
        {PREPOPULATED_ENGINE_ID_YAHOO_CH, &brave_yahoo_ch},
        {PREPOPULATED_ENGINE_ID_YAHOO_CL, &brave_yahoo_cl},
        {PREPOPULATED_ENGINE_ID_YAHOO_CO, &brave_yahoo_co},
        {PREPOPULATED_ENGINE_ID_YAHOO_DE, &brave_yahoo_de},
        {PREPOPULATED_ENGINE_ID_YAHOO_DK, &brave_yahoo_dk},
        {PREPOPULATED_ENGINE_ID_YAHOO_ES, &brave_yahoo_es},
        {PREPOPULATED_ENGINE_ID_YAHOO_FI, &brave_yahoo_fi},
        {PREPOPULATED_ENGINE_ID_YAHOO_FR, &brave_yahoo_fr},
        {PREPOPULATED_ENGINE_ID_YAHOO_HK, &brave_yahoo_hk},
        {PREPOPULATED_ENGINE_ID_YAHOO_ID, &brave_yahoo_id},
        {PREPOPULATED_ENGINE_ID_YAHOO_IN, &brave_yahoo_in},
        {PREPOPULATED_ENGINE_ID_YAHOO_JP, &brave_yahoo_jp},
        {PREPOPULATED_ENGINE_ID_YAHOO_MX, &brave_yahoo_mx},
        {PREPOPULATED_ENGINE_ID_YAHOO_MY, &brave_yahoo_my},
        {PREPOPULATED_ENGINE_ID_YAHOO_NL, &brave_yahoo_nl},
        {PREPOPULATED_ENGINE_ID_YAHOO_NZ, &brave_yahoo_nz},
        {PREPOPULATED_ENGINE_ID_YAHOO_PE, &brave_yahoo_pe},
        {PREPOPULATED_ENGINE_ID_YAHOO_PH, &brave_yahoo_ph},
        {PREPOPULATED_ENGINE_ID_YAHOO_QC, &brave_yahoo_qc},
        {PREPOPULATED_ENGINE_ID_YAHOO_SE, &brave_yahoo_se},
        {PREPOPULATED_ENGINE_ID_YAHOO_SG, &brave_yahoo_sg},
        {PREPOPULATED_ENGINE_ID_YAHOO_TH, &brave_yahoo_th},
        {PREPOPULATED_ENGINE_ID_YAHOO_TR, &brave_yahoo_tr},
        {PREPOPULATED_ENGINE_ID_YAHOO_TW, &brave_yahoo_tw},
        {PREPOPULATED_ENGINE_ID_YAHOO_UK, &brave_yahoo_uk},
        {PREPOPULATED_ENGINE_ID_YAHOO_VE, &brave_yahoo_ve},
        {PREPOPULATED_ENGINE_ID_YAHOO_VN, &brave_yahoo_vn},
        {PREPOPULATED_ENGINE_ID_BING, &bing},
        {PREPOPULATED_ENGINE_ID_DUCKDUCKGO, &duckduckgo},
        {PREPOPULATED_ENGINE_ID_DUCKDUCKGO_DE, &duckduckgo_de},
        {PREPOPULATED_ENGINE_ID_DUCKDUCKGO_AU_NZ_IE, &duckduckgo_au_nz_ie},
#if defined(OS_ANDROID)
        {PREPOPULATED_ENGINE_ID_DUCKDUCKGO_LITE, &duckduckgo_lite},
#endif
        {PREPOPULATED_ENGINE_ID_QWANT, &qwant},
        {PREPOPULATED_ENGINE_ID_STARTPAGE, &startpage},
};

// Default order in which engines will appear in the UI.
std::vector<BravePrepopulatedEngineID> brave_engines_default = {
    PREPOPULATED_ENGINE_ID_GOOGLE,
    PREPOPULATED_ENGINE_ID_YAHOO,
    PREPOPULATED_ENGINE_ID_DUCKDUCKGO,
#if defined(OS_ANDROID)
    PREPOPULATED_ENGINE_ID_DUCKDUCKGO_LITE,
#endif
    PREPOPULATED_ENGINE_ID_QWANT,
    PREPOPULATED_ENGINE_ID_BING,
    PREPOPULATED_ENGINE_ID_STARTPAGE,
};

// Germany - DDG appears on top.
std::vector<BravePrepopulatedEngineID> brave_engines_DE = {
    PREPOPULATED_ENGINE_ID_DUCKDUCKGO_DE,
    PREPOPULATED_ENGINE_ID_YAHOO,
#if defined(OS_ANDROID)
    PREPOPULATED_ENGINE_ID_DUCKDUCKGO_LITE,
#endif
    PREPOPULATED_ENGINE_ID_QWANT,
    PREPOPULATED_ENGINE_ID_GOOGLE,
    PREPOPULATED_ENGINE_ID_BING,
    PREPOPULATED_ENGINE_ID_STARTPAGE,
};

// France - Qwant appears on top.
std::vector<BravePrepopulatedEngineID> brave_engines_FR = {
    PREPOPULATED_ENGINE_ID_QWANT,
    PREPOPULATED_ENGINE_ID_YAHOO,
    PREPOPULATED_ENGINE_ID_GOOGLE,
    PREPOPULATED_ENGINE_ID_DUCKDUCKGO,
#if defined(OS_ANDROID)
    PREPOPULATED_ENGINE_ID_DUCKDUCKGO_LITE,
#endif
    PREPOPULATED_ENGINE_ID_BING,
    PREPOPULATED_ENGINE_ID_STARTPAGE,
};

// Australia / NZ / IE - DDG appears on top.
std::vector<BravePrepopulatedEngineID> brave_engines_AU_NZ_IE = {
    PREPOPULATED_ENGINE_ID_DUCKDUCKGO_AU_NZ_IE,
    PREPOPULATED_ENGINE_ID_YAHOO,
#if defined(OS_ANDROID)
    PREPOPULATED_ENGINE_ID_DUCKDUCKGO_LITE,
#endif
    PREPOPULATED_ENGINE_ID_GOOGLE,
    PREPOPULATED_ENGINE_ID_QWANT,
    PREPOPULATED_ENGINE_ID_BING,
    PREPOPULATED_ENGINE_ID_STARTPAGE};

// A map to keep track of default engines for countries that don't use the
// regular default engine.
const std::map<int, std::vector<BravePrepopulatedEngineID>>
    default_engines_by_country_id_map = {
        {country_codes::CountryCharsToCountryID('D', 'E'), brave_engines_DE},
        {country_codes::CountryCharsToCountryID('F', 'R'), brave_engines_FR},
        {country_codes::CountryCharsToCountryID('A', 'U'),
         brave_engines_AU_NZ_IE},
        {country_codes::CountryCharsToCountryID('N', 'Z'),
         brave_engines_AU_NZ_IE},
        {country_codes::CountryCharsToCountryID('I', 'E'),
         brave_engines_AU_NZ_IE}};

const std::map<int, BravePrepopulatedEngineID>
    yahoo_engines_by_country_id_map = {
        {country_codes::CountryCharsToCountryID('A', 'R'),
         PREPOPULATED_ENGINE_ID_YAHOO_AR},
        {country_codes::CountryCharsToCountryID('A', 'T'),
         PREPOPULATED_ENGINE_ID_YAHOO_AT},
        {country_codes::CountryCharsToCountryID('A', 'U'),
         PREPOPULATED_ENGINE_ID_YAHOO_AU},
        {country_codes::CountryCharsToCountryID('B', 'R'),
         PREPOPULATED_ENGINE_ID_YAHOO_BR},
        {country_codes::CountryCharsToCountryID('C', 'A'),
         PREPOPULATED_ENGINE_ID_YAHOO_CA},
        {country_codes::CountryCharsToCountryID('C', 'H'),
         PREPOPULATED_ENGINE_ID_YAHOO_CH},
        {country_codes::CountryCharsToCountryID('C', 'L'),
         PREPOPULATED_ENGINE_ID_YAHOO_CL},
        {country_codes::CountryCharsToCountryID('C', 'O'),
         PREPOPULATED_ENGINE_ID_YAHOO_CO},
        {country_codes::CountryCharsToCountryID('D', 'E'),
         PREPOPULATED_ENGINE_ID_YAHOO_DE},
        {country_codes::CountryCharsToCountryID('D', 'K'),
         PREPOPULATED_ENGINE_ID_YAHOO_DK},
        {country_codes::CountryCharsToCountryID('E', 'S'),
         PREPOPULATED_ENGINE_ID_YAHOO_ES},
        {country_codes::CountryCharsToCountryID('F', 'I'),
         PREPOPULATED_ENGINE_ID_YAHOO_FI},
        {country_codes::CountryCharsToCountryID('F', 'R'),
         PREPOPULATED_ENGINE_ID_YAHOO_FR},
        {country_codes::CountryCharsToCountryID('H', 'K'),
         PREPOPULATED_ENGINE_ID_YAHOO_HK},
        {country_codes::CountryCharsToCountryID('I', 'D'),
         PREPOPULATED_ENGINE_ID_YAHOO_ID},
        {country_codes::CountryCharsToCountryID('I', 'N'),
         PREPOPULATED_ENGINE_ID_YAHOO_IN},
        {country_codes::CountryCharsToCountryID('J', 'P'),
         PREPOPULATED_ENGINE_ID_YAHOO_JP},
        {country_codes::CountryCharsToCountryID('M', 'X'),
         PREPOPULATED_ENGINE_ID_YAHOO_MX},
        {country_codes::CountryCharsToCountryID('M', 'Y'),
         PREPOPULATED_ENGINE_ID_YAHOO_MY},
        {country_codes::CountryCharsToCountryID('N', 'L'),
         PREPOPULATED_ENGINE_ID_YAHOO_NL},
        {country_codes::CountryCharsToCountryID('N', 'Z'),
         PREPOPULATED_ENGINE_ID_YAHOO_NZ},
        {country_codes::CountryCharsToCountryID('P', 'E'),
         PREPOPULATED_ENGINE_ID_YAHOO_PE},
        {country_codes::CountryCharsToCountryID('P', 'H'),
         PREPOPULATED_ENGINE_ID_YAHOO_PH},
        {country_codes::CountryCharsToCountryID('Q', 'C'),
         PREPOPULATED_ENGINE_ID_YAHOO_QC},
        {country_codes::CountryCharsToCountryID('S', 'E'),
         PREPOPULATED_ENGINE_ID_YAHOO_SE},
        {country_codes::CountryCharsToCountryID('S', 'G'),
         PREPOPULATED_ENGINE_ID_YAHOO_SG},
        {country_codes::CountryCharsToCountryID('T', 'H'),
         PREPOPULATED_ENGINE_ID_YAHOO_TH},
        {country_codes::CountryCharsToCountryID('T', 'R'),
         PREPOPULATED_ENGINE_ID_YAHOO_TR},
        {country_codes::CountryCharsToCountryID('T', 'W'),
         PREPOPULATED_ENGINE_ID_YAHOO_TW},
        {country_codes::CountryCharsToCountryID('U', 'K'),
         PREPOPULATED_ENGINE_ID_YAHOO_UK},
        {country_codes::CountryCharsToCountryID('V', 'E'),
         PREPOPULATED_ENGINE_ID_YAHOO_VE},
        {country_codes::CountryCharsToCountryID('V', 'N'),
         PREPOPULATED_ENGINE_ID_YAHOO_VN}};

// Builds a vector of PrepulatedEngine objects from the given array of
// |engine_ids|. Fills in the default engine index for the given |country_id|,
// if asked.
std::vector<const PrepopulatedEngine*> GetEnginesFromEngineIDs(
    const std::vector<BravePrepopulatedEngineID>& engine_ids,
    int country_id,
    BravePrepopulatedEngineID default_engine_id,
    size_t* default_search_provider_index = nullptr) {
  std::vector<const PrepopulatedEngine*> engines;
  for (size_t i = 0; i < engine_ids.size(); ++i) {
    const PrepopulatedEngine* engine = brave_engines_map.at(engine_ids.at(i));
    DCHECK(engine);
    if (engine) {
      engines.push_back(engine);
      if (default_search_provider_index &&
          default_engine_id == engine_ids.at(i))
        *default_search_provider_index = i;
    }
  }
  return engines;
}

void UpdateTemplateURLDataKeyword(
    const std::unique_ptr<TemplateURLData>& t_urld) {
  DCHECK(t_urld.get());
  switch (t_urld->prepopulate_id) {
    case PREPOPULATED_ENGINE_ID_GOOGLE:
      t_urld->SetKeyword(base::ASCIIToUTF16(":g"));
      break;
    case PREPOPULATED_ENGINE_ID_BING:
      t_urld->SetKeyword(base::ASCIIToUTF16(":b"));
      break;
  }
}

// TODO(bsclifton): finish me
// void LocalizeEngineList(std::vector<BravePrepopulatedEngineID> engines) {
//   const auto& it = engines.find(PREPOPULATED_ENGINE_ID_YAHOO);
//   if (it != engines.end()) {
//     //..
//     // replace
//   }
// }

// Uses brave_engines_XX localized arrays of engine IDs instead of Chromium's
// localized arrays of PrepopulatedEngines to construct the vector of
// TemplateURLData. Also, fills in the default engine index for the given
// |country_id|.
std::vector<std::unique_ptr<TemplateURLData>>
GetBravePrepopulatedEnginesForCountryID(
    int country_id,
    size_t* default_search_provider_index = nullptr) {
  std::vector<BravePrepopulatedEngineID> brave_engines;

  // Check for exceptions from the default list of engines
  const auto& it = default_engines_by_country_id_map.find(country_id);
  if (it != default_engines_by_country_id_map.end()) {
    // brave_engines = LocalizeEngineList(it->second);
    brave_engines = it->second;
  } else {
    brave_engines = brave_engines_default;
  }
  DCHECK_GT(brave_engines.size(), 0ul);

  // Default engine is the first in the list
  BravePrepopulatedEngineID default_id = brave_engines.front();

  // Build a vector PrepopulatedEngines from BravePrepopulatedEngineIDs and
  // also get the default engine index
  std::vector<const PrepopulatedEngine*> engines =
      GetEnginesFromEngineIDs(brave_engines, country_id,
                              default_id, default_search_provider_index);
  DCHECK(engines.size() == brave_engines.size());

  std::vector<std::unique_ptr<TemplateURLData>> t_urls;
  for (const PrepopulatedEngine* engine : engines) {
    std::unique_ptr<TemplateURLData> t_urld =
        TemplateURLDataFromPrepopulatedEngine(*engine);
    UpdateTemplateURLDataKeyword(t_urld);
    t_urls.push_back(std::move(t_urld));
  }

  return t_urls;
}

}  // namespace

// Redefines function with the same name in Chromium. We need to account for
// the version of Brave engines as well: kCurrentDataVersion is defined in
// prepopulated_engines.json and is bumped every time the json file is
// modified. Since we add our own engines we need to keep track of our
// version as well and combine it with Chromium's version.
int GetDataVersion(PrefService* prefs) {
  int dataVersion = GetDataVersion_ChromiumImpl(prefs);
  // Check if returned version was from preferences override and if so return
  // that version.
  if (prefs && prefs->HasPrefPath(prefs::kSearchProviderOverridesVersion))
    return dataVersion;
  return (dataVersion + kBraveCurrentDataVersion);
}

// Redefines function with the same name in Chromium. Modifies the function to
// get search engines defined by Brave.
std::vector<std::unique_ptr<TemplateURLData>> GetPrepopulatedEngines(
    PrefService* prefs,
    size_t* default_search_provider_index) {
  // If there is a set of search engines in the preferences file, it overrides
  // the built-in set.
  if (default_search_provider_index)
    *default_search_provider_index = 0;
  std::vector<std::unique_ptr<TemplateURLData>> t_urls =
      GetPrepopulatedTemplateURLData(prefs);
  if (!t_urls.empty())
    return t_urls;

  return GetBravePrepopulatedEnginesForCountryID(
      country_codes::GetCountryIDFromPrefs(prefs),
      default_search_provider_index);
}

// Redefines function with the same name in Chromium. Modifies the function to
// get search engines defined by Brave.
#if defined(OS_ANDROID)

std::vector<std::unique_ptr<TemplateURLData>> GetLocalPrepopulatedEngines(
    const std::string& locale) {
  int country_id = country_codes::CountryStringToCountryID(locale);
  if (country_id == country_codes::kCountryIDUnknown) {
    LOG(ERROR) << "Unknown country code specified: " << locale;
    return std::vector<std::unique_ptr<TemplateURLData>>();
  }

  return GetBravePrepopulatedEnginesForCountryID(country_id);
}

#endif

SearchEngineType GetEngineType(const GURL& url) {
  SearchEngineType type = GetEngineType_ChromiumImpl(url);
  if (type == SEARCH_ENGINE_OTHER) {
    for (const auto& entry : brave_engines_map) {
      const auto* engine = entry.second;
      if (SameDomain(url, GURL(engine->search_url))) {
        return engine->type;
      }
      for (size_t j = 0; j < engine->alternate_urls_size; ++j) {
        if (SameDomain(url, GURL(engine->alternate_urls[j]))) {
          return engine->type;
        }
      }
    }
  }
  return type;
}

// Functions below are copied verbatim from
// components\search_engines\template_url_prepopulate_data.cc because they
// need to call our versions of redefined Chromium's functions.

std::unique_ptr<TemplateURLData> GetPrepopulatedEngine(PrefService* prefs,
                                                       int prepopulated_id) {
  size_t default_index;
  auto engines =
      TemplateURLPrepopulateData::GetPrepopulatedEngines(prefs, &default_index);
  for (auto& engine : engines) {
    if (engine->prepopulate_id == prepopulated_id)
      return std::move(engine);
  }
  return nullptr;
}

std::unique_ptr<TemplateURLData> GetPrepopulatedDefaultSearch(
    PrefService* prefs) {
  size_t default_search_index;
  // This could be more efficient.  We are loading all the URLs to only keep
  // the first one.
  std::vector<std::unique_ptr<TemplateURLData>> loaded_urls =
      GetPrepopulatedEngines(prefs, &default_search_index);

  return (default_search_index < loaded_urls.size())
             ? std::move(loaded_urls[default_search_index])
             : nullptr;
}

}  // namespace TemplateURLPrepopulateData

/*

Copyright (c) 2015, John Smith

Permission to use, copy, modify, and/or distribute this software for
any purpose with or without fee is hereby granted, provided that the
above copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR
BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES
OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

*/


#ifndef WOBBLYPROJECT_H
#define WOBBLYPROJECT_H

#ifndef PACKAGE_VERSION
#define PACKAGE_VERSION "9"
#endif

#ifndef PACKAGE_URL
#define PACKAGE_URL "https://github.com/Jaded-Encoding-Thaumaturgy/Wobbly"
#endif

#include <cstdint>

#include <unordered_map>
#include <map>

#include <set>

#include <array>
#include <vector>
#include <string>

#include <QObject>

#include "BookmarksModel.h"
#include "CombedFramesModel.h"
#include "CustomListsModel.h"
#include "FrozenFramesModel.h"
#include "PresetsModel.h"
#include "OrphanFieldsModel.h"
#include "SectionsModel.h"
#include "WobblyException.h"
#include "WobblyTypes.h"



static inline uint8_t matchCharToIndex(char match) {
    if (match == 'p')
        return 0;

    if (match == 'c')
        return 1;

    if (match == 'n')
        return 2;

    if (match == 'b')
        return 3;

    if (match == 'u')
        return 4;

    return 0; // never reaches this
}


static inline uint8_t matchCharToIndexDMetrics(char match) {
    if (match == 'c')
        return 1;

    if (match == 'n')
        return 2;

    if (match == 'b')
        return 0;

    return 0; // never reaches this
}

struct UndoStep {
    std::string description;

    std::vector<char> matches;
    std::vector<std::set<int8_t> > decimated_frames;
    PatternGuessing pattern_guessing;

    PresetMap presets;
    CustomListVector custom_lists;
    std::set<int> combed_frames;
    FreezeFrameMap frozen_frames;
    SectionMap sections;
    BookmarkMap bookmarks;
};


enum DecimationFunction {
    AUTO = 0,
    SELECTEVERY,
    DELETEFRAMES,
};

struct FinalScriptFormat {
    DecimationFunction decimation_function;
};

class WobblyProject : public QObject {
    Q_OBJECT

    private:
        int num_frames[2] = {};

        int64_t fps_num = 0;
        int64_t fps_den = 0;

        int width = 0;
        int height = 0;

        int zoom = 1;
        int last_visited_frame = 0;
        std::string ui_state;
        std::string ui_geometry;
        std::array<bool, 5> shown_frame_rates = {};
        int mic_search_minimum = 5;
        int dmetric_search_minimum = 10;
        int c_match_sequences_minimum = 20;

        std::string input_file;
        std::map<int, FrameRange> trims; // Key is FrameRange::first

        std::map<std::string, int> vfm_parameters_int;
        std::map<std::string, double> vfm_parameters_double;
        std::map<std::string, bool> vfm_parameters_bool;

        std::map<std::string, int> vdecimate_parameters_int;
        std::map<std::string, double> vdecimate_parameters_double;
        std::map<std::string, bool> vdecimate_parameters_bool;

        std::vector<std::array<int16_t, 5> > mics;
        std::vector<std::array<int32_t, 2> > mmetrics;
        std::vector<std::array<int32_t, 2> > vmetrics;
        std::vector<char> matches;
        std::vector<char> original_matches;
        std::vector<std::set<int8_t> > decimated_frames; // unordered_set may be sufficient.
        std::vector<int> decimate_metrics;

        bool is_wobbly; // XXX Maybe only the json writing function needs to know.

        PatternGuessing pattern_guessing;

        InterlacedFadeMap interlaced_fades; // Key is InterlacedFade::frame

        CombedFramesModel *combed_frames;
        OrphanFieldsModel *orphan_fields;
        FrozenFramesModel *frozen_frames;
        PresetsModel *presets;
        CustomListsModel *custom_lists;
        SectionsModel *sections;
        BookmarksModel *bookmarks;

        DMetrics dmetrics = { false, 10 };
        Resize resize = { false, 0, 0, "spline16" };
        Crop crop = {};
        Depth depth = { false, 8, false, "random" };

        std::string source_filter;

        bool freeze_frames_wanted = true;

        bool is_modified = false;

        std::list<UndoStep> undo_stack;
        std::list<UndoStep> redo_stack;
        size_t undo_steps;

        // Only functions below.

        static bool isValidMatchChar(char match);
        void setNumFrames(PositionInFilterChain position, int frames);

        bool isNameSafeForPython(const std::string &name) const;
        int maybeTranslate(int frame, bool is_end, PositionInFilterChain position) const;

        void applyPatternGuessingDecimation(const int section_start, const int section_end, const int first_duplicate, int drop_duplicate);

        void restoreState(UndoStep state);

    public:
        WobblyProject(bool _is_wobbly);
        WobblyProject(bool _is_wobbly, const std::string &_input_file, const std::string &_source_filter, int64_t _fps_num, int64_t _fps_den, int _width, int _height, int _num_frames);

        int getNumFrames(PositionInFilterChain position) const;

        void writeProject(const std::string &path, bool compact_project);
        void readProject(const std::string &path);


        void addFreezeFrame(int first, int last, int replacement);
        void deleteFreezeFrame(int frame);
        const FreezeFrame *findFreezeFrame(int frame) const;
        FrozenFramesModel *getFrozenFramesModel();


        void addPreset(const std::string &preset_name);
        void addPreset(const std::string &preset_name, const std::string &preset_contents);
        void renamePreset(const std::string &old_name, const std::string &new_name);
        void deletePreset(const std::string &preset_name);
        const std::string &getPresetContents(const std::string &preset_name) const;
        void setPresetContents(const std::string &preset_name, const std::string &preset_contents);
        bool isPresetInUse(const std::string &preset_name) const;
        bool presetExists(const std::string &preset_name) const;
        PresetsModel *getPresetsModel();


        void addTrim(int trim_start, int trim_end);


        void setVFMParameter(const std::string &name, int value);
        void setVFMParameter(const std::string &name, double value);
        void setVFMParameter(const std::string &name, bool value);

        void setVDecimateParameter(const std::string &name, int value);
        void setVDecimateParameter(const std::string &name, double value);
        void setVDecimateParameter(const std::string &name, bool value);

        std::array<int32_t, 3> getMMetrics(int frame) const;
        std::array<int32_t, 3> getVMetrics(int frame) const;
        std::array<int16_t, 5> getMics(int frame) const;
        void setMics(int frame, int16_t mic_p, int16_t mic_c, int16_t mic_n, int16_t mic_b, int16_t mic_u);
        void setDMetrics(int frame, int32_t mmetric_p, int32_t mmetric_c, int32_t vmetric_p, int32_t vmetric_c);
        int getPreviousFrameWithMic(int minimum, int start_frame) const;
        int getNextFrameWithMic(int minimum, int start_frame) const;
        int getPreviousFrameWithDMetric(int minimum, int start_frame) const;
        int getNextFrameWithDMetric(int minimum, int start_frame) const;


        char getOriginalMatch(int frame) const;
        void setOriginalMatch(int frame, char match);


        char getMatch(int frame) const;
        void setMatch(int frame, char match);
        void cycleMatchCNB(int frame);
        void cycleMatch(int frame);


        void addSection(int section_start);
        void addSection(const Section &section);
        void deleteSection(int section_start);
        const Section *findSection(int frame) const;
        const Section *findNextSection(int frame) const;
        int getSectionEnd(int frame) const;
        void setSectionPreset(int section_start, const std::string &preset_name);
        void deleteSectionPreset(int section_start, size_t preset_index);
        void moveSectionPresetUp(int section_start, size_t preset_index);
        void moveSectionPresetDown(int section_start, size_t preset_index);
        void setSectionMatchesFromPattern(int section_start, const std::string &pattern);
        void setSectionDecimationFromPattern(int section_start, const std::string &pattern);
        SectionsModel *getSectionsModel();


        void setRangeMatchesFromPattern(int range_start, int range_end, const std::string &pattern);
        void setRangeDecimationFromPattern(int range_start, int range_end, const std::string &pattern);


        void resetSectionMatches(int section_start);
        void resetRangeMatches(int start, int end);


        void addCustomList(const std::string &list_name);
        void addCustomList(const CustomList &list);
        void renameCustomList(const std::string &old_name, const std::string &new_name);
        void deleteCustomList(const std::string &list_name);
        void deleteCustomList(int list_index);
        void moveCustomListUp(int list_index);
        void moveCustomListDown(int list_index);
        const std::string &getCustomListPreset(int list_index) const;
        void setCustomListPreset(int list_index, const std::string &preset_name);
        PositionInFilterChain getCustomListPosition(int list_index) const;
        void setCustomListPosition(int list_index, PositionInFilterChain position);
        void addCustomListRange(int list_index, int first, int last);
        void deleteCustomListRange(int list_index, int first);
        const FrameRange *findCustomListRange(int list_index, int frame) const;
        bool customListExists(const std::string &list_name) const;
        bool isCustomListInUse(int list_index);
        CustomListsModel *getCustomListsModel();


        int getDecimateMetric(int frame) const;
        void setDecimateMetric(int frame, int decimate_metric);


        void addDecimatedFrame(int frame);
        void deleteDecimatedFrame(int frame);
        bool isDecimatedFrame(int frame) const;
        void clearDecimatedFramesFromCycle(int frame);


        DecimationRangeVector getDecimationRanges() const;
        DecimationPatternRangeVector getDecimationPatternRanges() const;


        std::map<size_t, size_t> getCMatchSequences(int minimum) const;

        void updateOrphanFields();
        void updateSectionOrphanFields(int section_start, int section_end);

        CombedFramesModel *getCombedFramesModel();
        void addCombedFrame(int frame);
        void deleteCombedFrame(int frame);
        bool isCombedFrame(int frame) const;
        void clearCombedFrames();

        OrphanFieldsModel *getOrphanFieldsModel();
        void deleteOrphanField(int frame);
        bool isOrphanField(int frame) const;
        void clearOrphanFields();

        const Resize &getResize() const;
        void setResize(int new_width, int new_height, const std::string &filter);
        void setResizeEnabled(bool enabled);
        bool isResizeEnabled() const;

        const Crop &getCrop() const;
        void setCrop(int left, int top, int right, int bottom);
        void setCropEnabled(bool enabled);
        bool isCropEnabled() const;
        void setCropEarly(bool early);
        bool isCropEarly() const;

        const Depth &getBitDepth() const;
        void setBitDepth(int bits, bool float_samples, const std::string &dither);
        void setBitDepthEnabled(bool enabled);
        bool isBitDepthEnabled() const;


        const std::string &getSourceFilter() const;
        void setSourceFilter(const std::string &filter);


        bool getFreezeFramesWanted() const;
        void setFreezeFramesWanted(bool wanted);


        bool isModified() const;
        void setModified(bool modified);


        // If these are the empty string, there is no undo/redo action available
        std::string getUndoDescription();
        std::string getRedoDescription();

        void commit(std::string description);
        void undo();
        void redo();
        void setUndoSteps(size_t steps);


        int getZoom() const;
        void setZoom(int ratio);


        int getLastVisitedFrame() const;
        void setLastVisitedFrame(int frame);


        std::string getUIState() const;
        void setUIState(const std::string &state);


        std::string getUIGeometry() const;
        void setUIGeometry(const std::string &geometry);


        std::array<bool, 5> getShownFrameRates() const;
        void setShownFrameRates(const std::array<bool, 5> &rates);


        int getMicSearchMinimum() const;
        void setMicSearchMinimum(int minimum);

        int getDMetricSearchMinimum() const;
        void setDMetricSearchMinimum(int minimum);

        int getCMatchSequencesMinimum() const;
        void setCMatchSequencesMinimum(int minimum);


        std::string frameToTime(int frame) const;


        int frameNumberAfterDecimation(int frame) const;
        int frameNumberBeforeDecimation(int frame) const;


        bool guessSectionPatternsFromMics(int section_start, int minimum_length, int use_patterns, int drop_duplicate);
        void guessProjectPatternsFromMics(int minimum_length, int use_patterns, int drop_duplicate);

        bool guessSectionPatternsFromDMetrics(int section_start, int minimum_length, int use_patterns, int drop_duplicate);
        void guessProjectPatternsFromDMetrics(int minimum_length, int use_patterns, int drop_duplicate);

        bool guessSectionPatternsFromMicsAndDMetrics(int section_start, int minimum_length, int use_patterns, int drop_duplicate);
        void guessProjectPatternsFromMicsAndDMetrics(int minimum_length, int use_patterns, int drop_duplicate);

        bool guessSectionPatternsFromMatches(int section_start, int minimum_length, int use_third_n_match, int drop_duplicate);
        void guessProjectPatternsFromMatches(int minimum_length, int use_third_n_match, int drop_duplicate);
        const PatternGuessing &getPatternGuessing();


        void addInterlacedFade(int frame, double field_difference);
        const InterlacedFadeMap &getInterlacedFades() const;


        void addBookmark(int frame, const std::string &description);
        void deleteBookmark(int frame);
        bool isBookmark(int frame) const;
        int findPreviousBookmark(int frame) const;
        int findNextBookmark(int frame) const;
        const Bookmark *getBookmark(int frame) const;
        BookmarksModel *getBookmarksModel();

        int findPreviousCombedFrame(int frame) const;
        int findNextCombedFrame(int frame) const;

        int findPreviousAmbiguousPatternSection(int frame) const;
        int findNextAmbiguousPatternSection(int frame) const;

        void sectionsToScript(std::string &script) const;
        void customListsToScript(std::string &script, PositionInFilterChain position) const;
        void headerToScript(std::string &script) const;
        void presetsToScript(std::string &script) const;
        const char *getArgsForSourceFilter() const;
        void sourceToScript(std::string &script, bool save_node) const;
        void trimToScript(std::string &script) const;
        void fieldHintToScript(std::string &script) const;
        void freezeFramesToScript(std::string &script) const;
        void decimatedFramesToScript(std::string &script, DecimationFunction decimation_function) const;
        void cropToScript(std::string &script) const;
        void resizeAndBitDepthToScript(std::string &script, bool resize_enabled, bool depth_enabled) const;
        void setOutputToScript(std::string &script) const;

        std::string generateFinalScript(bool save_source_node = true, FinalScriptFormat format = {}) const;
        std::string generateMainDisplayScript() const;

        std::string generateTimecodesV1() const;
        std::string generateKeyframesV1() const;


        void importFromOtherProject(const std::string &path, const ImportedThings &imports);

    signals:
        void modifiedChanged(bool modified);
};

#endif // WOBBLYPROJECT_H

/////////////////////////////////////////////////////////////////////////////
// Name:        rest.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_REST_H__
#define __VRV_REST_H__

#include "atts_externalsymbols.h"
#include "atts_mensural.h"
#include "durationinterface.h"
#include "layerelement.h"
#include "positioninterface.h"

namespace vrv {

class TransPitch;

enum RestLayer { RL_UNSET = -1, RL_sameLayer, RL_otherLayer };

enum RestAccidental { RA_UNSET = -1, RA_none, RA_s, RA_f, RA_x, RA_n };

enum RestLayerPlace { RLP_UNSET = -1, RLP_restOnTopLayer, RLP_restOnBottomLayer };

enum RestNotePlace { RNP_UNSET = -1, RNP_noteInSpace, RNP_noteOnLine };

//----------------------------------------------------------------------------
// Rest
//----------------------------------------------------------------------------

/**
 * This class models the MEI <rest> element.
 */
class Rest : public LayerElement,
             public DurationInterface,
             public PositionInterface,
             public AttColor,
             public AttCue,
             public AttExtSym,
             public AttRestVisMensural {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    Rest();
    virtual ~Rest();
    Object *Clone() const override { return new Rest(*this); }
    void Reset() override;
    std::string GetClassName() const override { return "Rest"; }
    ///@}

    /**
     * Add an element to a rest.
     * Only Dots elements will be actually added to the rest.
     */
    bool IsSupportedChild(Object *object) override;

    /**
     * Overwritten method for rest
     */
    void AddChild(Object *object) override;

    /**
     * @name Getter to interfaces
     */
    ///@{
    PositionInterface *GetPositionInterface() override { return dynamic_cast<PositionInterface *>(this); }
    DurationInterface *GetDurationInterface() override { return dynamic_cast<DurationInterface *>(this); }
    ///@}

    /** Override the method since alignment is required */
    bool HasToBeAligned() const override { return true; }

    /**
     * Get the SMuFL glyph or a rest considering its actual duration.
     * This is valid only for CMN and for duration shorter than half notes.
     */
    wchar_t GetRestGlyph() const;

    /**
     * MIDI timing information
     */
    ///@{
    void SetScoreTimeOnset(double scoreTime);
    void SetRealTimeOnsetSeconds(double timeInSeconds);
    void SetScoreTimeOffset(double scoreTime);
    void SetRealTimeOffsetSeconds(double timeInSeconds);
    void SetScoreTimeTiedDuration(double timeInSeconds);
    void CalcMIDIPitch(int shift);
    double GetScoreTimeOnset() const;
    double GetRealTimeOnsetMilliseconds() const;
    double GetScoreTimeOffset() const;
    double GetScoreTimeTiedDuration() const;
    double GetRealTimeOffsetMilliseconds() const;
    double GetScoreTimeDuration() const;
    char GetMIDIPitch() const;
    ///@}

    //----------//
    // Functors //
    //----------//

    /**
     * See Object::AdjustBeams
     */
    int AdjustBeams(FunctorParams *functorParams) override;

    /**
     * See Object::ConvertMarkupAnalytical
     */
    int ConvertMarkupAnalytical(FunctorParams *functorParams) override;

    /**
     * See Object::CalcDots
     */
    int CalcDots(FunctorParams *functorParams) override;

    /**
     * See Object::PrepareLayerElementParts
     */
    int PrepareLayerElementParts(FunctorParams *functorParams) override;

    /**
     * See Object::ResetDrawing
     */
    int ResetDrawing(FunctorParams *functorParams) override;

    /**
     * See Object::ResetHorizontalAlignment
     */
    int ResetHorizontalAlignment(FunctorParams *functorParams) override;

    /**
     * See Object::Transpose
     */
    int Transpose(FunctorParams *functorParams) override;

    /**
     * Get the vertical location for the rests that are located on other layers
     */
    int GetOptimalLayerLocation(Staff *staff, Layer *layer, int defaultLocation);

    /**
     * // SS See Object::GenerateTimemap
     */
    int GenerateTimemap(FunctorParams *functorParams) override;

private:
    /**
     * Helper function to update rest oloc/ploc based on the input TransPitch
     */
    void UpdateFromTransLoc(const TransPitch &tp);

    /**
     * Get the rest vertical location relative to location of elements placed on other layers
     */
    std::pair<int, RestAccidental> GetLocationRelativeToOtherLayers(
        const ListOfObjects &layersList, Layer *currentLayer, bool isTopLayer);

    /**
     * Get the rest vertical location relative to location of elements placed on current layers
     */
    int GetLocationRelativeToCurrentLayer(Staff *currentStaff, Layer *currentLayer, bool isTopLayer);

    /**
     * Get location of first/last element of the corresponding layer
     */
    int GetFirstRelativeElementLocation(Staff *currentStaff, Layer *currentLayer, bool isPrevious, bool isTopLayer);

    /**
     * Get location of the object on the layer if it's note, chord or ftrem
     */
    std::pair<int, RestAccidental> GetElementLocation(Object *object, Layer *layer, bool isTopLayer) const;

    /**
     * Get correct offset for the rest from the options based on layer and location
     */
    int GetRestOffsetFromOptions(
        RestLayer layer, const std::pair<int, RestAccidental> &location, bool isTopLayer) const;

public:
    //
private:
    /**
     * The score-time onset of the note in the measure (duration from the start of measure in
     * quarter notes).
     */
    double m_scoreTimeOnset;

    /**
     * The score-time off-time of the note in the measure (duration from the start of the measure
     * in quarter notes).  This is the duration of the printed note.  If the note is the start of
     * a tied group, the score time of the tied group is this variable plus m_scoreTimeTiedDuration.
     * If this note is a secondary note in a tied group, then this value is the score time end
     * of the printed note, and the m_scoreTimeTiedDuration is -1.0 to indicate that it should not
     * be exported when creating a MIDI file.
     */
    double m_scoreTimeOffset;

    /**
     * The time in milliseconds since the start of the measure element that contains the note.
     */
    double m_realTimeOnsetMilliseconds;

    /**
     * The time in milliseconds since the start of the measure element to end of printed note.
     * The real-time duration of a tied group is not currently tracked (this gets complicated
     * if there is a tempo change during a note sustain, which is currently not supported).
     */
    double m_realTimeOffsetMilliseconds;

    /**
     * If the note is the first in a tied group, then m_scoreTimeTiedDuration contains the
     * score-time duration (in quarter notes) of all tied notes in the group after this note.
     * If the note is a secondary note in a tied group, then this variable is set to -1.0 to
     * indicate that it should not be written to MIDI output.
     */
    double m_scoreTimeTiedDuration;
};

} // namespace vrv

#endif

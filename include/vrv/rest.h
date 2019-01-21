/////////////////////////////////////////////////////////////////////////////
// Name:        rest.h
// Author:      Laurent Pugin
// Created:     2011
// Copyright (c) Authors and others. All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef __VRV_REST_H__
#define __VRV_REST_H__

#include "atts_mensural.h"
#include "durationinterface.h"
#include "layerelement.h"
#include "positioninterface.h"

namespace vrv {

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
             public AttRestVisMensural {
public:
    /**
     * @name Constructors, destructors, reset and class name methods
     * Reset method resets all attribute classes
     */
    ///@{
    Rest();
    virtual ~Rest();
    virtual void Reset();
    virtual std::string GetClassName() const { return "Rest"; }
    virtual ClassId GetClassId() const { return REST; }
    ///@}

    /**
     * Add an element to a rest.
     * Only Dots elements will be actually added to the rest.
     */
    virtual void AddChild(Object *object);

    /**
     * @name Getter to interfaces
     */
    ///@{
    virtual PositionInterface *GetPositionInterface() { return dynamic_cast<PositionInterface *>(this); }
    virtual DurationInterface *GetDurationInterface() { return dynamic_cast<DurationInterface *>(this); }
    ///@}

    /** Override the method since alignment is required */
    virtual bool HasToBeAligned() const { return true; }

    /**
     * Get the SMuFL glyph or a rest considering its actual duration.
     * This is valid only for CMN and for duration shorter than half notes.
     */
    wchar_t GetRestGlyph() const;

    /**
     * Get the vertical offset for each glyph.
     */
    int GetRestLocOffset(int loc);

// SS BEGIN
                 
     /**
      * MIDI timing information
      */
     ///@{
     void SetScoreTimeOnset(double scoreTime);
     void SetRealTimeOnsetSeconds(double timeInSeconds);
     void SetScoreTimeOffset(double scoreTime);
     void SetRealTimeOffsetSeconds(double timeInSeconds);
     void SetScoreTimeTiedDuration(double timeInSeconds);
     double GetScoreTimeOnset();
     int GetRealTimeOnsetMilliseconds();
     double GetScoreTimeOffset();
     double GetScoreTimeTiedDuration();
     int GetRealTimeOffsetMilliseconds();
     double GetScoreTimeDuration();
     ///@}

// SS END
                 
    //----------//
    // Functors //
    //----------//

    /**
     * See Object::ConvertAnalyticalMarkup
     */
    virtual int ConvertAnalyticalMarkup(FunctorParams *functorParams);

    /**
     * See Object::CalcDots
     */
    virtual int CalcDots(FunctorParams *functorParams);

    /**
     * See Object::PrepareLayerElementParts
     */
    virtual int PrepareLayerElementParts(FunctorParams *functorParams);

    /**
     * See Object::ResetDrawing
     */
    virtual int ResetDrawing(FunctorParams *functorParams);

    /**
     * See Object::ResetHorizontalAlignment
     */
    virtual int ResetHorizontalAlignment(FunctorParams *functorParams);

    // SS
    /**
    * See Object::GenerateTimemap
    */
    virtual int GenerateTimemap(FunctorParams *functorParams);
                 

                 
private:
    //
public:
    //
private:
// SS BEGIN

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
                 int m_realTimeOnsetMilliseconds;
                 
                 /**
                  * The time in milliseconds since the start of the measure element to end of printed note.
                  * The real-time duration of a tied group is not currently tracked (this gets complicated
                  * if there is a tempo change during a note sustain, which is currently not supported).
                  */
                 int m_realTimeOffsetMilliseconds;
                 
                 /**
                  * If the note is the first in a tied group, then m_scoreTimeTiedDuration contains the
                  * score-time duration (in quarter notes) of all tied notes in the group after this note.
                  * If the note is a secondary note in a tied group, then this variable is set to -1.0 to
                  * indicate that it should not be written to MIDI output.
                  */
                 double m_scoreTimeTiedDuration;

// SS END
};

} // namespace vrv

#endif

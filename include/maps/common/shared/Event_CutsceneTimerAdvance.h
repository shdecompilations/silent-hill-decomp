/** @brief Advances a cutscene timer between `[startTime, endTime]` at the given rate, 
 * optionally advancing the state step when the end time is reached.
 *
 * @note If the timer is below `startTime`, it is clamped to `startTime` and no advancement occurs.
 * Otherwise, the timer increments by `g_DeltaTime * rate` each frame.
 * When the timer exceeds `endTime`:
 *   - If `clampToEnd` was `true`, the timer is set to exactly `endTime`.
 *   - If `autoAdvance` was `true`, the cutscene state step is incremented.
 *
 * @param timer Cutscene timer, typically `g_Cutscene_Timer`. Represents the current DMS timeline position.
 * @param rate Rate of timer advancement in DMS frames per second (scaled by `g_DeltaTime` each frame).
 * @param startTime Start of the active range. Timer is clamped up to this value.
 * @param endTime End of the active range. Timer stops advancing beyond this point.
 * @param clampToEnd If `true`, clamp the timer to `endTime` when reached.
 * @param autoAdvance If `true`, increment the cutscene state step when `endTime` is reached.
 */
void Event_CutsceneTimerAdvance(q19_12* timer, q19_12 rate, q19_12 startTime, q19_12 endTime,
                                bool clampToEnd, bool autoAdvance)
{
    // Clamp timer to min.
    if (*timer < startTime)
    {
        *timer = startTime;
        return;
    }

    // Increment timer.
    *timer += Q12_MULT_PRECISE(g_DeltaTime, rate);

    // End timer and increment state step.
    if (*timer > endTime)
    {
        if (clampToEnd)
        {
            *timer = endTime;
        }

        if (autoAdvance)
        {
            SysWork_StateStepIncrement(0);
        }
    }
}

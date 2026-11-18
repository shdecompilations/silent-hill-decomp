#ifndef _VERSION_H
#define _VERSION_H

/** @brief Defines region and revision-specific version macros based on the `VER_XXX` Makefile target.
 *
 * Supports both region-gated code and date-based gating where changes are known to propagate
 * forward across releases.
 *
 * Three styles of version check macros are defined:
 *
 * - `VERSION_IS(USA)` — matches a single release exactly. Use for things that
 *   differ between every build, e.g. random padding bytes or raw addresses.
 *
 * - `VERSION_REGION_IS(NTSC)` — matches all releases in a region. Use for
 *   region-specific behavior, e.g. Japanese text rendering/
 *
 * - `VERSION_EQUAL_OR_NEWER(USA)` / `VERSION_EQUAL_OR_OLDER(JAP0)` — matches
 *   by build date. Use for changes introduced in one release and carried
 *   forward, e.g. bugfixes.
 */

/** @brief Checks if the build target is a specific release.
 *
 * @param release Release identifier (JAP0, USA, JAP1, EUR, JAP2).
 */
#define VERSION_IS(release) defined(VER_##release)

/** @brief Checks if the build target matches a given region.
 *
 * @param region Region identifier (NTSC, NTSCJ, PAL).
 */
#define VERSION_REGION_IS(region) defined(VERSION_##region)

/** @brief Checks if the build target version is equal to or newer than the given release.
 *
 * @param release Release to compare against (PROTO_981216, JAP0, USA, JAP1, EUR, JAP2).
 */
#define VERSION_EQUAL_OR_NEWER(release) (VERSION_DATE >= VERSION_DATE_##release)

/** @brief Checks if the build target version is equal to or older than the given release.
 *
 * @param release Release to compare against (PROTO_981216, JAP0, USA, JAP1, EUR, JAP2).
 */
#define VERSION_EQUAL_OR_OLDER(release) (VERSION_DATE <= VERSION_DATE_##release)

// Version defines that code can test against.
#define VERSION_DATE_PROTO_981216  981216
#define VERSION_DATE_PROTO_990107  990107
#define VERSION_DATE_JAP0          990126
#define VERSION_DATE_USA           990210
#define VERSION_DATE_JAP1          990602
#define VERSION_DATE_EUR           990607
#define VERSION_DATE_JAP2          990616

#if defined(VER_JAP0)
    // Silent Hill (JP) v1.0
    #define VERSION_NTSCJ 1
    #define VERSION_DATE VERSION_DATE_JAP0
    #define VERSION_SERIAL "SLPM-86192"
#elif defined(VER_USA)
    // Silent Hill (US) v1.1
    #define VERSION_NTSC 1
    #define VERSION_DATE VERSION_DATE_USA
    #define VERSION_SERIAL "SLUS-00707"
#elif defined(VER_JAP1)
    // Silent Hill (JP) v1.01
    #define VERSION_NTSCJ 1
    #define VERSION_DATE VERSION_DATE_JAP1
    #define VERSION_SERIAL "SLPM-86192" // Actually SLPM-86498, but regions share same serial for save support.
#elif defined(VER_EUR)
    // Silent Hill (EU) v1.0
    #define VERSION_PAL 1
    #define VERSION_DATE VERSION_DATE_EUR
    #define VERSION_SERIAL "SLES-01514"
#elif defined(VER_JAP2)
    // Silent Hill (JP) v1.1
    #define VERSION_NTSCJ 1
    #define VERSION_DATE VERSION_DATE_JAP2
    #define VERSION_SERIAL "SLPM-86192" // Actually SLPM-87029, but regions share same serial for save support.
#endif

/** @brief Checks the language of a PAL map overlay.
 *
 * PAL includes copies of each map overlay per-language, only difference appears to be translated
 * map messages.
 * For these copies we can build from the same source with Makefile defining `LANG_XXX`, which this
 * macro checks for.
 * 
 * Messages can then be switched out by checking `#if LANGUAGE_IS(ENG)` inside map message code.
 *
 * TODO: We should handle JPN text the same way to be consistent, needs more Makefile changes though.
 *
 * @param lang Language identifier (ENG, GER, FRN, SPN, ITL).
 */
#define LANGUAGE_IS(lang) defined(LANG_##lang)

// @hack Remove once all versions build with LANG_ defines set.
#if VERSION_REGION_IS(PAL) && !defined(LANG_GER) && !defined(LANG_FRN) && !defined(LANG_SPN) && !defined(LANG_ITL)
    #define LANG_ENG 1
#endif

#endif

#ifndef DEWALLS_WALLSPARSER_H
#define DEWALLS_WALLSPARSER_H

#include <QString>
#include <QHash>
#include <QSharedPointer>
#include <QStack>
#
#include "lineparser.h"
#include "unitizeddouble.h"
#include "length.h"
#include "angle.h"
#include "cardinaldirection.h"
#include "varianceoverride.h"
#include "wallstypes.h"
#include "wallsunits.h"
#include "wallsvisitor.h"

namespace dewalls {

class WallsParser : public LineParser
{
public:
    typedef UnitizedDouble<Length> ULength;
    typedef UnitizedDouble<Angle>  UAngle;
    typedef const Unit<Length> * LengthUnit;
    typedef const Unit<Angle>  * AngleUnit;
    typedef QSharedPointer<VarianceOverride> VarianceOverridePtr;
    typedef void (WallsParser::*OwnProduction)();

    WallsParser();
    WallsParser(QString line);
    WallsParser(Segment segment);

    WallsVisitor* visitor() const;
    void setVisitor(WallsVisitor* visitor);

    QSharedPointer<WallsUnits> units() const;
    QDate date() const;
    QString segment() const;

    ULength unsignedLengthInches();
    ULength unsignedLengthNonInches(LengthUnit defaultUnit);
    ULength unsignedLength(LengthUnit defaultUnit);
    ULength length(LengthUnit defaultUnit);

    UAngle unsignedAngle(QHash<QChar, AngleUnit> unitSuffixes, AngleUnit defaultUnit);
    UAngle unsignedDmsAngle();

    UAngle latitude();
    UAngle longitude();

    UAngle nonQuadrantAzimuth(AngleUnit defaultUnit);
    UAngle quadrantAzimuth();
    UAngle azimuth(AngleUnit defaultUnit);
    UAngle azimuthOffset(AngleUnit defaultUnit);

    UAngle unsignedInclination(AngleUnit defaultUnit);
    UAngle inclination(AngleUnit defaultUnit);

    VarianceOverridePtr varianceOverride(LengthUnit defaultUnit);
    VarianceOverridePtr floatedVectorVarianceOverride();
    VarianceOverridePtr floatedTraverseVarianceOverride();
    VarianceOverridePtr lengthVarianceOverride(LengthUnit defaultUnit);
    VarianceOverridePtr rmsErrorVarianceOverride(LengthUnit defaultUnit);

    template<typename F>
    QChar escapedChar(F charPredicate, std::initializer_list<QString> expectedItems);
    template<typename F>
    QString escapedText(F charPredicate, std::initializer_list<QString> expectedItems);
    QString quotedTextOrNonwhitespace();
    QString quotedText();

    template<typename R, typename F>
    bool optional(R& result, F production);

    template<typename T>
    QList<T> elementChars(QHash<QChar, T> elements, QSet<T> requiredElements);

    void parseLine();
    void parseLine(QString line);
    void parseLine(Segment line);

    void beginBlockCommentLine();
    void endBlockCommentLine();
    void insideBlockCommentLine();

    void directiveLine();
    void segmentLine();
    void prefixLine();
    void noteLine();
    void flagLine();
    void symbolLine();
    void dateLine();
    void unitsLine();
    void vectorLine();

    QString combineSegments(QString base, Segment offset);

    WallsParser& operator=(const WallsParser& rhs);

private:
    static double approx(double val);

    static QHash<QString, OwnProduction> createUnitsOptionMap();
    static QHash<QString, OwnProduction> createDirectivesMap();

    const QHash<QString, LengthUnit> lengthUnits;
    const QHash<QString, AngleUnit> azmUnits;
    const QHash<QString, AngleUnit> incUnits;
    const QHash<QChar, LengthUnit> lengthUnitSuffixes;
    const QHash<QChar, AngleUnit> azmUnitSuffixes;
    const QHash<QChar, AngleUnit> incUnitSuffixes;
    const QHash<QChar, CardinalDirection> cardinalDirections;
    const QHash<QChar, CardinalDirection> northSouth;
    const QHash<QChar, CardinalDirection> eastWest;
    const QHash<QChar, QChar> escapedChars;
    const QHash<QChar, CtElement> ctElements;
    const QSet<CtElement> requiredCtElements;
    const QHash<QChar, RectElement> rectElements;
    const QSet<RectElement> requiredRectElements;
    const QHash<QChar, LrudElement> lrudElements;
    const QSet<LrudElement> requiredLrudElements;
    const QHash<QString, bool> correctedValues;
    const QHash<QString, CaseType> caseTypes;
    const QHash<QString, LrudType> lrudTypes;
    const QHash<QString, QList<TapingMethodElement>> tapingMethods;
    const QHash<QString, int> prefixDirectives;

    const QRegExp wordRx;
    const QRegExp notSemicolonRx;
    const QRegExp unitsOptionRx;
    const QRegExp directiveRx;
    const QRegExp macroNameRx;
    const QRegExp stationRx;
    const QRegExp prefixRx;

    const QRegExp optionalRx;
    const QRegExp optionalStationRx;

    const QRegExp isoDateRx;
    const QRegExp usDateRx1;
    const QRegExp usDateRx2;
    const QRegExp usDateRx3;

    const QHash<QString, OwnProduction> unitsOptionMap;
    const QHash<QString, OwnProduction> directivesMap;

    static const UAngle oneEighty;

    UAngle azmDifference(UAngle fs, UAngle bs);
    UAngle incDifference(UAngle fs, UAngle bs);

    void replaceMacros();
    QString movePastEndQuote();
    QString replaceMacro();

    Segment untilComment(std::initializer_list<QString> expectedItems);

    Segment segmentDirective();
    void prefixDirective();
    void noteDirective();

    void flagDirective();
    QString slashPrefixedFlag();

    QDate dateDirective();
    QDate isoDate();
    QDate usDate1();
    QDate usDate2();
    QDate usDate3();

    void unitsOptions();
    void unitsOption();
    void macroOption();
    void save();
    void restore();
    void reset_();
    void meters();
    void feet();
    void ct();
    void d();
    void s();
    void a();
    void ab();
    void a_ab();
    void v();
    void vb();
    void v_vb();
    void order();
    void ctOrder();
    void rectOrder();
    void decl();
    void grid();
    void rect();
    void incd();
    void inch();
    void incs();
    void inca();
    void incab();
    void incv();
    void incvb();
    void typeab();
    void typevb();
    void case_();
    void lrud();
    void lrudOrder();
    void prefix1();
    void prefix2();
    void prefix3();
    void prefix(int index);
    void tape();
    void uvh();
    void uvv();
    void uv();
    void flag();

    void fromStation();
    void afterFromStation();
    void toStation();
    void afterToStation();
    void rectElement(RectElement elem);
    void east();
    void north();
    void rectUp();
    void ctElement(CtElement elem);
    void distance();
    void azimuth();
    void inclination();
    void tapingMethodElements();
    void tapingMethodElement(TapingMethodElement elem);
    void instrumentHeight();
    void targetHeight();
    void lrudElement(LrudElement elem);
    void left();
    void right();
    void up();
    void down();
    void afterVectorMeasurements();
    void varianceOverrides();
    void afterVectorVarianceOverrides();
    void lruds();
    void lrudContent();
    void commaDelimLrudContent();
    void whitespaceDelimLrudContent();
    void afterRequiredCommaDelimLrudMeasurements();
    void afterRequiredWhitespaceDelimLrudMeasurements();
    void lrudFacingAngle();
    void lrudCFlag();
    void afterLruds();
    void inlineDirective();
    void inlineSegmentDirective();
    void fixLine();
    void fixedStation();
    void afterFixedStation();
    void fixRectElement(RectElement elem);
    void fixEast();
    void fixNorth();
    void fixUp();
    void afterFixMeasurements();
    void afterFixVarianceOverrides();
    void inlineNote();
    void afterFixInlineNote();
    void comment();

    void inlineCommentOrEndOfLine();
    void inlineComment();

    WallsVisitor* _visitor;
    bool _inBlockComment;
    QSharedPointer<WallsUnits> _units;
    QStack<QSharedPointer<WallsUnits>> _stack;
    QString _segment;
    QDate _date;

    Segment _fromStationSegment;
    QString _fromStation;
    Segment _toStationSegment;
    QString _toStation;

    Segment _azmSegment;
    UAngle  _azmFs;
    UAngle  _azmBs;

    Segment _incSegment;
    UAngle  _incFs;
    UAngle  _incBs;
};

inline WallsVisitor* WallsParser::visitor() const
{
    return _visitor;
}

inline void WallsParser::setVisitor(WallsVisitor* visitor)
{
    _visitor = visitor;
}

inline QSharedPointer<WallsUnits> WallsParser::units() const
{
    return _units;
}

inline QDate WallsParser::date() const
{
    return _date;
}

inline QString WallsParser::segment() const
{
    return _segment;
}

template<typename F>
QChar WallsParser::escapedChar(F charPredicate, std::initializer_list<QString> expectedItems)
{
    QChar c = expectChar(charPredicate, expectedItems);
    return c == '\\' ? oneOfMap(escapedChars) : c;
}

template<typename F>
QString WallsParser::escapedText(F charPredicate, std::initializer_list<QString> expectedItems)
{
    QString result;
    while (maybe([&]() { result.append(this->escapedChar(charPredicate, expectedItems)); } ));
    return result;
}

template<typename R, typename F>
bool WallsParser::optional(R& result, F production)
{
    if (maybe([&]() { return this->expect(optionalRx, {"-", "--"}); }))
    {
        return false;
    }
    result = production();
    return true;
}

template<typename T>
QList<T> WallsParser::elementChars(QHash<QChar, T> elements, QSet<T> requiredElements)
{
    QList<T> result;
    while (!elements.isEmpty())
    {
        T element;
        if (requiredElements.isEmpty()) {
            if (!maybe(element, [&]() { return this->oneOfMap(elements); }))
            {
                break;
            }
        }
        else {
            element = oneOfMap(elements);
        }
        result += element;
        QChar c = _line.at(_i - 1);
        elements.remove(c.toLower());
        elements.remove(c.toUpper());
        requiredElements -= element;
    }
    return result;
}

} // namespace dewalls

#endif // DEWALLS_WALLSPARSER_H

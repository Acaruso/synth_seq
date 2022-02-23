#include "synth_settings_elt.hpp"

#include "src/main/ui_elements/advanced/number_with_label_elt.hpp"
#include "src/main/util.hpp"

void _numberElt(AppContext& context, std::string label, Coord coord, std::string key);

void synthSettingsElt(EltParams& params)
{
    AppContext& context = params.ctx;
    Font& font = context.graphicsWrapper.getFont(params.fontName);
    Coord numCoord = params.coord;

    int numXPadding = 200;
    int numYPadding = 6;

    // col 1
    _numberElt(
        context,
        "volume",
        numCoord,
        "volume"
    );

    numCoord.y += font.lineHeight + numYPadding;
    _numberElt(
        context,
        "mod amount",
        numCoord,
        "modAmount"
    );

    // col 2
    numCoord.x += numXPadding;
    numCoord.y = params.coord.y;
    _numberElt(
        context,
        "attack",
        numCoord,
        "attack"
    );

    numCoord.y += font.lineHeight + numYPadding;
    _numberElt(
        context,
        "hold",
        numCoord,
        "hold"
    );

    numCoord.y += font.lineHeight + numYPadding;
    _numberElt(
        context,
        "release",
        numCoord,
        "release"
    );

    // col3
    numCoord.x += numXPadding;
    numCoord.y = params.coord.y;
    _numberElt(
        context,
        "mod attack",
        numCoord,
        "modAttack"
    );

    numCoord.y += font.lineHeight + numYPadding;
    _numberElt(
        context,
        "mod hold",
        numCoord,
        "modHold"
    );

    numCoord.y += font.lineHeight + numYPadding;
    _numberElt(
        context,
        "mod release",
        numCoord,
        "modRelease"
    );
}

void _numberElt(AppContext& context, std::string label, Coord coord, std::string key)
{
    EltParams p(context);
    p.label = label;
    p.coord = coord;

    auto& synthSettings = context.sequencer->getSynthSettings();

    int& data = synthSettings[key];

    p.min = 0;
    p.max = 101;

    int maxNumDigits = (int)std::to_string(p.max).size();

    p.displayText = pad(maxNumDigits, std::to_string(data));

    p.onDrag = [&]() {
        int drag = context.getDragAmount();
        data = (drag == 1 || drag == -1) ? data + drag : data + (drag / 2);
        data = clamp(data, p.min, p.max);
    };

    numberWithLabelElt(p);
}

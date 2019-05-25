open FC__Base;

let styles = (~isDisabled=false, ~heightPadding=2, ()) => {
  let main =
    Css.(
      style([
        padding2(~v=`px(heightPadding), ~h=`px(14)),
        BaseStyles.floatLeft,
        borderRadius(Colors.BorderRadius.medium),
        border(`px(1), `solid, Colors.accentBlueO8),
      ])
    );
  let disabledStyles = Css.(style([background(Colors.greydisabled)]));
  isDisabled ? Css.merge([disabledStyles, main]) : main;
};

module TagLink = {
  let make =
    Link.make(~className=styles(~heightPadding=4, ()), ~isDisabled=false);
};

let withNumber = (~isActive, ~text, ~number: int) => {
  let textStyle =
    Css.(
      style([
        BaseStyles.floatLeft,
        marginRight(`em(0.7)),
        marginTop(`px(3)),
      ])
    );

  let colors =
    Colors.Text.(
      isActive ?
        Css.[
          color(LightBackground.active),
          hover([color(LightBackground.active)]),
        ] :
        Css.[
          color(LightBackground.main),
          hover([color(LightBackground.active)]),
        ]
    );

  <Link
    className=Css.(
      style(
        [BaseStyles.floatLeft, padding2(~v=`em(0.7), ~h=`em(1.2))] @ colors,
      )
    )
    isDisabled=false>
    <span className=textStyle> text </span>
    <span className={styles()}>
      {number |> string_of_int |> ReasonReact.string}
    </span>
  </Link>;
};
[@bs.config {jsx: 3}];

module Styles = {
  open Css;
  let textOverlay = style([position(`absolute)]);
  let mainText = style([fontSize(`em(1.1)), color(FC__Settings.darkLink)]);
  let secondaryText =
    style([fontSize(`em(0.9)), color(FC__Settings.accentBlue)]);
};

[@react.component]
let make = (~cdf: FC__Types.Dist.t, ~showMean=true) =>
  <>
    {showMean
       ? <div className=Styles.mainText>
           <FC__NumberShower
             precision=2
             number={cdf |> FC__Types.Dist.findX(0.5)}
           />
         </div>
       : ReasonReact.null}
    <div className=Styles.secondaryText>
      <FC__NumberShower
        precision=2
        number={cdf |> FC__Types.Dist.findX(0.05)}
      />
      {" to " |> ReasonReact.string}
      <FC__NumberShower
        precision=2
        number={cdf |> FC__Types.Dist.findX(0.95)}
      />
    </div>
  </>;

module Jsx2 = {
  let make = (~cdf: FC__Types.Dist.t, ~showMean=true, children) =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~cdf, ~showMean, ()),
      children,
    );
};
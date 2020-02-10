type item = {
  name: string,
  icon: string,
  href: string,
  bookmark: bool,
  onClick: ReactEvent.Mouse.t => unit,
  onBookmark: ReactEvent.Mouse.t => unit,
};

module Styles = {
  open Css;
  let item =
    style([
      flex(`num(1.)),
      textDecoration(`none),
      padding4(
        ~top=`em(0.3),
        ~left=`em(1.5),
        ~right=`em(1.5),
        ~bottom=`em(0.3),
      ),
      hover([background(Settings.buttonHover)]),
      display(`flex),
      flexDirection(`row),
    ]);

  let header =
    style([
      flex(`num(1.)),
      textDecoration(`none),
      fontSize(`rem(0.8)),
      padding4(
        ~top=`em(1.5),
        ~left=`em(1.8),
        ~right=`em(1.8),
        ~bottom=`em(0.9),
      ),
      color(`hex("999")),
      display(`flex),
    ]);

  let itemIcon =
    style([
      color(`hex("7e8aa1")),
      fontSize(`rem(1.1)),
      marginTop(`em(-0.1)),
    ]);

  let itemText = style([color(`hex("262c37"))]);

  let notBookmarkedIcon =
    style([
      hover([color(`hex("7e8aa1"))]),
      color(`hex("f0f1f4")),
      fontSize(`rem(1.1)),
      marginTop(`em(-0.1)),
    ]);

  let bookmarkedIcon =
    style([
      hover([color(`hex("7e8aa1"))]),
      color(`hex("7e8aa1")),
      fontSize(`rem(1.1)),
      marginTop(`em(-0.1)),
    ]);
};

module Item = {
  [@react.component]
  let make = (~item) => {
    let bookmarkStyle =
      item.bookmark ? Styles.bookmarkedIcon : Styles.notBookmarkedIcon;

    <a href={item.href} onClick={item.onClick} className=Styles.item>
      <Div flex={`num(1.)} className=Styles.itemIcon>
        <ReactKitIcon icon={item.icon} />
      </Div>
      <Div flex={`num(7.)} className=Styles.itemText>
        {item.name |> ReasonReact.string}
      </Div>
      <Div flex={`num(0.5)} className=bookmarkStyle>
        <ReactKitIcon icon="STAR_FULL" />
      </Div>
    </a>;
  };
};

module Header = {
  [@react.component]
  let make = (~name: string) => {
    <Div flex={`num(1.)} className=Styles.header>
      {name |> ReasonReact.string}
    </Div>;
  };
};

[@react.component]
let make = (~children) => {
  <Div flexDirection=`column> children </Div>;
};
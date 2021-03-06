let valueFromEvent = (evt): string => evt->ReactEvent.Form.target##value;

module Styles = {
  open Css;

  let input = style([height(`em(10.)), width(`percent(100.0))]);
  let padding = style([padding(`em(1.0))]);
};

[@react.component]
let make = () => {
  let (text, setText) = React.useState(() => "");

  <SLayout container=`fluid>
    <ForetoldComponents.PageCard.Body>
      <div className=Styles.padding>
        <Antd.Input.TextArea
          style={ReactDOMRe.Style.make(~minHeight="6em", ())}
          onChange={event => setText(_ => valueFromEvent(event))}
          value=text
        />
      </div>
      <Markdown source=text supportForetoldJs=true />
    </ForetoldComponents.PageCard.Body>
  </SLayout>;
};
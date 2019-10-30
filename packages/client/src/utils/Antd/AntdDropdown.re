[@bs.module "./AntdDropdown.js"]
external dropdown: ReasonReact.reactClass = "Dropdown";

[%bs.raw {|require("antd/lib/dropdown/style")|}];

[@bs.obj]
external makeProps:
  (~overlay: ReasonReact.reactElement=?, ~overlayClassName: string=?, unit) =>
  _ =
  "";

let make = (~overlay=?, ~overlayClassName=?, children) =>
  ReasonReact.wrapJsForReason(
    ~reactClass=dropdown,
    ~props=makeProps(~overlay?, ~overlayClassName?, ()),
    children,
  );
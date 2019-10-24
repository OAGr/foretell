[@bs.config {jsx: 3}];

module Functor = (Ken: KenTools.KenModule) => {
  let kenDisplay = id => {
    Ken.names(id)
    |> E.A.of_list
    |> E.A.fmapi((i, r: BsKen.Graph_T.T.fact) =>
         <div key={i |> string_of_int}>
           {Ken.findName(r.propertyId)
            |> E.O.default("no-name")
            |> Utils.ste
            |> E.React.inH3}
           BsKen.Graph_T.T.(
             switch (r.value.valueType) {
             | String(s) => s |> Utils.ste
             | ThingId(s) =>
               <Link.Jsx2 linkType={Internal(EntityShow(s))}>
                 {s |> Utils.ste}
               </Link.Jsx2>
             | _ => "no-name" |> Utils.ste
             }
           )
         </div>
       )
    |> ReasonReact.array;
  };

  let xEntityLink = (attribute, ~m: Types.measurable, ~className: string) =>
    m
    |> attribute
    |> E.O.bind(_, Ken.findName)
    |> E.O.bind(_, r =>
         m
         |> attribute
         |> E.O.fmap(_d =>
              <AntdPopover
                content={kenDisplay(attribute(m) |> E.O.default(""))}
                trigger=`hover
                placement=`top>
                <span className> {r |> Utils.ste} </span>
              </AntdPopover>
            )
       );

  let nameEntityLink = xEntityLink(r => r.labelSubject);
  let propertyEntityLink = xEntityLink(r => r.labelProperty);
};

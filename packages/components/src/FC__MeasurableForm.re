open FC__Base;

module PageCard = FC__PageCard;
module Tab = FC__Tab;
module TabList = FC__TabList;

// Could this be in some flexrow or part of Div.Jsx2 component?
let flexRowContainer =
  Css.(style([margin2(~v=`zero, ~h=`px(-6)), alignItems(`flexEnd)]));
let flexRowItem = Css.(style([margin2(~v=`zero, ~h=`px(6))]));

type tabs =
  | SimpleTab
  | FreeformTab
  | CustomTab;

type state = {selectedTab: tabs};

type action =
  | ChangeTab(tabs);

let component = ReasonReact.reducerComponent(__MODULE__);
let make = (~cdf: FC__Types.Dist.t, _children) => {
  ...component,
  initialState: () => {selectedTab: SimpleTab},
  reducer: (action: action, _state: state) => {
    switch (action) {
    | ChangeTab(tab) => ReasonReact.Update({selectedTab: tab})
    };
  },
  render: self =>
    <PageCard.Jsx2>
      <PageCard.HeaderRow.Jsx2>
        <PageCard.HeaderRow.Title.Jsx2>
          "New Prediction"->React.string
          <FC__HelpDropdown.Jsx2
            content={
              headerContent: "sdf" |> ReasonReact.string,
              bodyContent: "sdfsdfsd" |> ReasonReact.string,
            }
          />
        </PageCard.HeaderRow.Title.Jsx2>
      </PageCard.HeaderRow.Jsx2>
      <PageCard.Section.Jsx2 flex=true padding=`none>
        <TabList.Jsx2
          selected={self.state.selectedTab}
          onClick={key => self.send(ChangeTab(key))}
          list=[
            (SimpleTab, "Simple"),
            (FreeformTab, "Free-form"),
            (CustomTab, "Custom"),
          ]
          flex=true
        />
      </PageCard.Section.Jsx2>
      <PageCard.Section.Jsx2 background=`grey border=`bottom padding=`top>
        <FC__CdfChart__Large.Jsx2 cdf width=None />
      </PageCard.Section.Jsx2>
      <PageCard.Section.Jsx2 background=`grey>
        {switch (self.state.selectedTab) {
         | SimpleTab =>
           <Div.Jsx2 flexDirection=`row styles=[flexRowContainer]>
             <Div.Jsx2 flex={`num(1.0)} styles=[flexRowItem]>
               <InputLabel.Jsx2> "Min"->React.string </InputLabel.Jsx2>
               <TextInput.Jsx2 fullWidth=true />
             </Div.Jsx2>
             <Div.Jsx2 flex={`num(1.0)} styles=[flexRowItem]>
               <InputLabel.Jsx2> "Max"->React.string </InputLabel.Jsx2>
               <TextInput.Jsx2 fullWidth=true />
             </Div.Jsx2>
             <Div.Jsx2 styles=[flexRowItem]>
               <Button.Jsx2 variant=Button.Secondary>
                 "Clear"->React.string
               </Button.Jsx2>
             </Div.Jsx2>
           </Div.Jsx2>
         | FreeformTab =>
           <TextInput.Jsx2 fullWidth=true placeholder="5 to 50" />
         | CustomTab =>
           <div>
             <div>
               <DropdownSelect
                 initialValue={Some("CDF")}
                 values=[("CDF", "CDF"), ("PDF", "PDF")]
               />
               <Icon.Questionmark.Jsx2 />
             </div>
             <PageCard.VerticalSpace.Jsx2 />
             <Alert.Jsx2 type_=`error>
               "Input is not a valid PDF"->React.string
             </Alert.Jsx2>
             <PageCard.VerticalSpace.Jsx2 />
             <TextArea.Jsx2 rows=4 fullWidth=true />
           </div>
         }}
      </PageCard.Section.Jsx2>
      <PageCard.Section.Jsx2>
        <InputLabel.Jsx2> "Comment"->React.string </InputLabel.Jsx2>
        <TextArea.Jsx2 fullWidth=true />
        <PageCard.VerticalSpace.Jsx2 />
        <Button.Jsx2 variant=Button.Primary fullWidth=true size=Large>
          "Submit Prediction"->React.string
        </Button.Jsx2>
      </PageCard.Section.Jsx2>
    </PageCard.Jsx2>,
};

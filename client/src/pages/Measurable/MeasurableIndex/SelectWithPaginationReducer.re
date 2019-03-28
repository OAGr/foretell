open Utils;
open Foretold__GraphQL;
open Rationale.Function.Infix;
open Rationale.Option.Infix;

module Types = {
  type pageNumber = int;

  type itemUnselected = {pageNumber: int};

  type itemSelected = {
    pageNumber,
    selectedIndex: int,
  };

  type itemState =
    | ItemUnselected(itemUnselected)
    | ItemSelected(itemSelected);

  type response =
    Client.E.HttpResponse.t(array(Context.Primary.Measurable.t));

  type action =
    | UpdateResponse(response)
    | NextPage
    | LastPage
    | Deselect
    | SelectIndex(int)
    | NextSelection
    | LastSelection;

  let reducerName = action =>
    switch (action) {
    | NextPage => "NextPage"
    | LastPage => "LastPage"
    | Deselect => "Deselect"
    | SelectIndex(int) => "SelectIndex"
    | NextSelection => "nextSelection"
    | LastSelection => "LastSelection"
    | _ => "Other"
    };

  type send = action => unit;

  type reducerParams = {
    itemsPerPage: int,
    itemState,
    response: option(response),
    send,
  };

  type state = {
    itemState,
    response: option(response),
  };
};

open Types;

let itemsPerPage = 20;

module Reducers = {
  module ItemSelected = {
    type t = itemSelected;

    let deselect = (_, t: t) => ItemUnselected({pageNumber: t.pageNumber});

    let nextSelection = (itemsPerPage, t: t) =>
      E.BoundedInt.increment(t.selectedIndex, itemsPerPage)
      <$> (
        selectedIndex =>
          ItemSelected({pageNumber: t.pageNumber, selectedIndex})
      );

    let lastSelection = (itemsPerPage, t: t) =>
      E.BoundedInt.decrement(t.selectedIndex, itemsPerPage)
      <$> (
        selectedIndex =>
          ItemSelected({pageNumber: t.pageNumber, selectedIndex})
      );

    let newState = (itemsPerPage, t: t, action: action) =>
      (
        switch (action) {
        | Deselect => Some(((a, b) => deselect(a, b) |> E.O.some))
        | NextSelection => Some(nextSelection)
        | LastSelection => Some(lastSelection)
        | _ => None
        }
      )
      |> (
        e =>
          switch (e) {
          | Some(fn) => fn(itemsPerPage, t)
          | None => None
          }
      );
  };

  module ItemUnselected = {
    type t = itemUnselected;

    let nextPage = (_, t: t) =>
      t.pageNumber
      |> E.NonZeroInt.increment
      <$> (p => ItemUnselected({pageNumber: p}));

    let lastPage = (_, t: t) =>
      t.pageNumber
      |> E.NonZeroInt.decrement
      <$> (p => ItemUnselected({pageNumber: p}));

    let selectIndex = (i, itemsPerPage, t: t) =>
      E.BoundedInt.make(i, itemsPerPage)
      <$> (s => ItemSelected({pageNumber: t.pageNumber, selectedIndex: s}));

    let newState = (itemsPerPage, t: t, action: action) =>
      (
        switch (action) {
        | NextPage => Some(nextPage)
        | LastPage => Some(lastPage)
        | SelectIndex(i) => Some(selectIndex(i))
        | _ => None
        }
      )
      |> (
        e =>
          switch (e) {
          | Some(fn) => fn(itemsPerPage, t)
          | None => None
          }
      );
  };

  module ItemState = {
    type t = itemState;
    let pageNumber = (t: t) =>
      switch (t) {
      | ItemSelected(r) => r.pageNumber
      | ItemUnselected(r) => r.pageNumber
      };
  };

  module ReducerParams = {
    type t = reducerParams;
    let pageNumber = (t: t) =>
      switch (t.itemState) {
      | ItemSelected(r) => r.pageNumber
      | ItemUnselected(r) => r.pageNumber
      };
    let pageIndex = (t: t) =>
      switch (t.itemState) {
      | ItemSelected(r) => Some(r.selectedIndex)
      | ItemUnselected(_) => None
      };

    let canDecrementPage = (t: t) => t |> pageNumber > 0;
    let canIncrementPage = (itemsOnPage, t: t) =>
      itemsOnPage == t.itemsPerPage;
  };
};

let component = ReasonReact.reducerComponent("PaginationReducer");

let compareMeasurables = (a, b) =>
  Belt.Array.eq(a, b, Context.Primary.Measurable.isEqual);

let make = (~itemsPerPage=20, ~channelId: string, ~subComponent, _children) => {
  ...component,
  initialState: () => {
    itemState: ItemUnselected({pageNumber: 0}),
    response: None,
  },
  reducer: (action, state: state) =>
    switch (action) {
    | UpdateResponse(response) =>
      {response: Some(response), itemState: state.itemState}
      ->ReasonReact.Update
    | _ =>
      let newState =
        switch (state, action) {
        | ({itemState: ItemUnselected(s)}, _) =>
          Reducers.ItemUnselected.newState(itemsPerPage, s, action)
        | ({itemState: ItemSelected(s)}, _) =>
          Reducers.ItemSelected.newState(itemsPerPage, s, action)
        };
      switch (newState) {
      | Some(s) =>
        {response: state.response, itemState: s}->ReasonReact.Update
      | None => ReasonReact.NoUpdate
      };
    },
  render: ({state, send}) =>
    Queries.Measurables.component2(
      channelId,
      state.itemState |> Reducers.ItemState.pageNumber,
      itemsPerPage,
      response => {
        switch (state.response) {
        | Some(r) =>
          if (!E.HttpResponse.isEq(r, response, compareMeasurables)) {
            send(UpdateResponse(response));
          }
        | None => send(UpdateResponse(response))
        };
        subComponent(
          ~selectWithPaginationParams={
            itemsPerPage,
            itemState: state.itemState,
            response: state.response,
            send,
          },
        );
      },
    ),
};
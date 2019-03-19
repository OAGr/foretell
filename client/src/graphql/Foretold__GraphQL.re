module Queries = {
  module Agent = Foretold__GraphQL__GetAgent;
  module Agents = Foretold__GraphQL__GetAgents;
  module Measurable = Foretold__GraphQL__GetMeasurable;
  module Measurables = Foretold__GraphQL__GetMeasurables;
  module MeasurableWithMeasurements = Foretold__GraphQL__GetMeasurableWithMeasurements;
  module Series = Foretold__GraphQL__GetSeries;
  module User = Foretold__GraphQL__GetUser;
  module SeriesCollection = Foretold__GraphQL__GetSeriesCollection;
};

module Mutations = {
  module MeasurementCreate = Foretold__GraphQL__CreateMeasurementMutation;
};
[@bs.module]
external introspect: Js.Json.t = "../../../../graphql_schema.json";
let introspectQ = introspect;
let data: Js.Json.t = [%bs.raw {| introspectQ.default.data |}];
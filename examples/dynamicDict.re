/*
    Handling an object with dynamic keys for sub-objects.
    example:
    {
        static: "hello",
        dynamics: {
            "undetermined1": 2
            "undetermined2": 6
        }
    }

    Where the "undetermined" keys, are unknown at compile time.
    Could be dynamic JS keys generated by user or generally at run time.
*/

type obj = {
  static: string,
  dynamics: Js.Dict.t(int)
};

module Decode = {
  let obj = json =>
    Json.Decode.{
      static: json |> field("static", string),
      dynamics: json |> field("dynamics", dict(int)),
    };
};

module Encode = {
  let obj = (c) => {
    Json.Encode.(
      object_([
        ("static", string(c.static)),
        ("dynamics", c.dynamics |> Js.Dict.map((. value) => int(value)) |> dict),
      ])
    );
  };
};

let data = {| {
  "static": "hi",
  "dynamics": { "hello": 5, "random": 8 }
} |};

let decodedData =
  data |> Json.parseOrRaise
       |> Decode.obj;

/*
Will log [ 'hi', { hello: 5, random: 8 } ]
*/
let _ =
  decodedData |> Js.log;

/*
Will log { static: 'hi', dynamics: { hello: 5, random: 8 } }
*/
let encodedDataBack =
  decodedData |> Encode.obj
              |> Js.log;
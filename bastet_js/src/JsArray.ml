(** This module provdes [Js.Array]-sepcific implementations for those who want things to compile into
    clean javascript code. You can still use {!Array} on the JS side if this doesn't matter to you. *)

open Bastet

module A = ArrayF.Make (struct
  let length = Js.Array.length

  let make n value =
    let arr = [||] in
    for _ = 1 to n do
      Js.Array.push ~value arr |> ignore
    done;
    arr

  let append = Belt.Array.concat

  let map f x = Js.Array.map ~f x

  let mapi f x = Js.Array.mapi ~f x

  let fold_left f init x = Js.Array.reduce ~f ~init x

  let every f x = Js.Array.every ~f x

  let slice ~start ~end_ x = Js.Array.slice ~start ~end_ x
end)

include A

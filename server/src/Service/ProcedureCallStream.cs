using System;
using System.Diagnostics.CodeAnalysis;
using System.Linq;
using KRPC.Continuations;
using KRPC.Server;
using KRPC.Service.Messages;
using KRPC.Service.Scanner;

namespace KRPC.Service
{
    [SuppressMessage ("Gendarme.Rules.Naming", "UseCorrectSuffixRule")]
    sealed class ProcedureCallStream : Stream
    {
        ProcedureSignature procedure;
        object[] arguments;

        public ProcedureCallStream (ProcedureCall call)
        {
            var services = Services.Instance;
            procedure = services.GetProcedureSignature (call.Service, call.Procedure);
            arguments = services.GetArguments (procedure, call.Arguments);
        }

        public override bool Equals (Stream other)
        {
            if (ReferenceEquals (other, null))
                return false;
            var obj = other as ProcedureCallStream;
            if (ReferenceEquals (obj, null))
                return false;
            return procedure == obj.procedure &&
                Enumerable.SequenceEqual (arguments, obj.arguments);
        }

        public override int GetHashCode ()
        {
            return procedure.GetHashCode ();
        }

        public override void Update() {
            try {
                Result = Service.Services.Instance.ExecuteCall (procedure, arguments);
            } catch (RPCException e) {
                Result = new ProcedureResult { Error = Core.HandleException (e) };
            } catch (YieldException e) {
                // FIXME: handle yields correctly
                Result = new ProcedureResult { Error = Core.HandleException (e) };
            }
        }
    }
}

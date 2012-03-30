//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2006-2008 OpenSim Ltd
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

import org.omnetpp.queueing.Classifier;
import org.omnetpp.queueing.Queue;
import org.omnetpp.queueing.Server;
import org.omnetpp.queueing.Sink;
import org.omnetpp.queueing.Source;
import org.omnetpp.queueing.Trigger;


//
// A call center simulation.
//
network WRS1
{
    parameters:
        @display("i=device/cellphone;bgb=846,838");
    submodules:
        source: Source {
            @display("p=41,399");
        }
        classifier: Classifier {
            @display("p=136,399");
        }
        queue0: Queue {
            @display("p=335,88");
        }
        queue1: Queue {
            @display("p=335,183");
        }
        queue2: Queue {
            @display("p=335,276");
        }
        queue3: Queue {
            @display("p=335,360");
        }
        server: Server {
            @display("p=582,399");
        }
        sink: Sink {
            @display("p=766,399");
        }
        queue4: Queue {
            @display("p=335,462");
        }
        queue5: Queue {
            @display("p=335,568");
        }
        queue6: Queue {
            @display("p=335,669");
        }
        queue7: Queue {
            @display("p=335,781");
        }
        trigger: Trigger {
            @display("p=582,207;b=36,10");
        }
    connections:
        source.out --> classifier.in++;
        classifier.out++ --> queue0.in++;
        classifier.out++ --> queue1.in++;
        classifier.out++ --> queue2.in++;
        classifier.out++ --> queue3.in++;

        trigger.out --> server.in++;

        queue0.out++ --> server.in++;
        queue1.out++ --> server.in++;
        queue2.out++ --> server.in++;
        queue3.out++ --> server.in++;
        classifier.out++ --> queue4.in++;
        classifier.out++ --> queue5.in++;
        classifier.out++ --> queue6.in++;
        classifier.out++ --> queue7.in++;
        queue4.out++ --> server.in++;
        queue5.out++ --> server.in++;
        queue6.out++ --> server.in++;
        queue7.out++ --> server.in++;

        server.out --> sink.in++;
}

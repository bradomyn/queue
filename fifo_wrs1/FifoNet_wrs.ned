//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2008 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

//import org.omnetpp.queueing.WRS;



//
// Simple queueing network: generator + FIFO + sink.
//
network FifoNetWRS
{
    @display("bgb=933,647");
    submodules:
        gen0: WRSource {
            parameters:
                @display("p=83,100");
        }
        gen1: WRSource {
            parameters:
                @display("p=83,174");
        }
        gen2: WRSource {
            parameters:
                @display("p=83,240");
        }
        gen3: WRSource {
            parameters:
                @display("p=83,307");
        }
        gen4: WRSource {
            parameters:
                @display("p=83,368");
        }
        gen5: WRSource {
            parameters:
                @display("p=83,442");
        }
        gen6: WRSource {
            parameters:
                @display("p=83,508");
        }
        gen7: WRSource {
            parameters:
                @display("p=83,575");
        }
        sink0: WRSink {
            parameters:
                @display("p=886,346");
        }
        wrClassifier_in: WRClassifier_in {
            @display("p=334,329;b=28,30");
        }
        passiveQueue0: PassiveQueue {
            @display("p=529,100;b=29,18");
        }
        passiveQueue1: PassiveQueue {
            @display("p=529,174;b=29,18");
        }
        server: Server {
            @display("p=807,346");
        }
        passiveQueue2: PassiveQueue {
            @display("p=529,220;b=29,18");
        }
        passiveQueue3: PassiveQueue {
            @display("p=529,300;b=29,18");
        }
        passiveQueue4: PassiveQueue {
            @display("p=529,376;b=29,18");
        }
        passiveQueue5: PassiveQueue {
            @display("p=529,456;b=29,18");
        }
        passiveQueue6: PassiveQueue {
            @display("p=529,524;b=29,18");
        }
        passiveQueue7: PassiveQueue {
            @display("p=529,604;b=29,18");
        }
    connections:
        gen0.out --> wrClassifier_in.in++;
        gen1.out --> wrClassifier_in.in++;
        gen2.out --> wrClassifier_in.in++;
        gen3.out --> wrClassifier_in.in++;
        gen4.out --> wrClassifier_in.in++;
        gen5.out --> wrClassifier_in.in++;
        gen6.out --> wrClassifier_in.in++;
        gen7.out --> wrClassifier_in.in++;


        //wrs.out[0] --> sink.in;
        //wrs.out[1] --> sink1.in;
        //wrs.out[2] --> sink3.in;
        //wrs.out[3] --> sink2.in;
        //wrs.out[4] --> sink5.in;
        //wrs.out[5] --> sink4.in;
        //wrs.out[6] --> sink7.in;
        //wrs.out[7] --> sink6.in;
        wrClassifier_in.out++ --> passiveQueue0.in;
        //wrs.out[0] --> sink.in;
        //wrs.out[1] --> sink1.in;
        //wrs.out[2] --> sink3.in;
        //wrs.out[3] --> sink2.in;
        //wrs.out[4] --> sink5.in;
        //wrs.out[5] --> sink4.in;
        //wrs.out[6] --> sink7.in;
        //wrs.out[7] --> sink6.in;
        wrClassifier_in.out++ --> passiveQueue1.in;
        passiveQueue0.out++ --> server.in++;
        passiveQueue1.out++ --> server.in++;
        server.out --> sink0.in++;
        //wrs.out[0] --> sink.in;
        //wrs.out[1] --> sink1.in;
        //wrs.out[2] --> sink3.in;
        //wrs.out[3] --> sink2.in;
        //wrs.out[4] --> sink5.in;
        //wrs.out[5] --> sink4.in;
        //wrs.out[6] --> sink7.in;
        //wrs.out[7] --> sink6.in;
        wrClassifier_in.out++ --> passiveQueue2.in;
        passiveQueue2.out++ --> server.in++;
        wrClassifier_in.out++ --> passiveQueue3.in;
        wrClassifier_in.out++ --> passiveQueue4.in;
        wrClassifier_in.out++ --> passiveQueue5.in;
        wrClassifier_in.out++ --> passiveQueue6.in;
        wrClassifier_in.out++ --> passiveQueue7.in;
        passiveQueue3.out++ --> server.in++;
        passiveQueue4.out++ --> server.in++;
        passiveQueue5.out++ --> server.in++;
        passiveQueue6.out++ --> server.in++;
        passiveQueue7.out++ --> server.in++;
}



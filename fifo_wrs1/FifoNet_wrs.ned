//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2008 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

//import org.omnetpp.queueing.WRS;


import org.omnetpp.queueing.WRClassifier_in;


//
// Simple queueing network: generator + FIFO + sink.
//
network FifoNetWRS
{
    @display("bgb=422,615");
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
                @display("p=329,100");
        }
        sink1: WRSink {
            parameters:
                @display("p=329,162");
        }
        sink2: WRSink {
            parameters:
                @display("p=329,220");
        }
        sink3: WRSink {
            parameters:
                @display("p=329,282");
        }
        sink4: WRSink {
            parameters:
                @display("p=329,345");
        }
        sink5: WRSink {
            parameters:
                @display("p=329,407");
        }
        sink6: WRSink {
            parameters:
                @display("p=329,474");
        }
        sink7: WRSink {
            parameters:
                @display("p=329,536");
        }
        wrs: WRS {
            @display("p=219,162;b=48,234");
        }
    connections:
        gen0.out --> wrs.in++;
        gen1.out --> wrs.in++;
        gen2.out --> wrs.in++;
        gen3.out --> wrs.in++;
        gen4.out --> wrs.in++;
        gen5.out --> wrs.in++;
        gen6.out --> wrs.in++;
        gen7.out --> wrs.in++;

        wrs.out++ --> sink0.in;
        wrs.out++ --> sink1.in;
        wrs.out++ --> sink3.in;
        wrs.out++ --> sink2.in;
        wrs.out++ --> sink5.in;
        wrs.out++ --> sink4.in;
        wrs.out++ --> sink7.in;
        wrs.out++ --> sink6.in;


        //gen.out --> wrs.in[0];
        //gen1.out --> wrs.in[1];
        //gen2.out --> wrs.in[2];
        //gen3.out --> wrs.in[3];
        //gen4.out --> wrs.in[4];
        //gen5.out --> wrs.in[5];
        //gen6.out --> wrs.in[6];
        //gen7.out --> wrs.in[7];

        //wrs.out[0] --> sink.in;
        //wrs.out[1] --> sink1.in;
        //wrs.out[2] --> sink3.in;
        //wrs.out[3] --> sink2.in;
        //wrs.out[4] --> sink5.in;
        //wrs.out[5] --> sink4.in;
        //wrs.out[6] --> sink7.in;
        //wrs.out[7] --> sink6.in;
}



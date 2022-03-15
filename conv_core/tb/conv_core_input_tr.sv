class conv_core_input_tr;
    rand int signed ima[49];
    rand int signed wei[49];
    rand int signed bias;

    //special case
    constraint c{
        foreach(ima[u]){
            ima[u] == u;
        }
        foreach(wei[j]){
            wei[j] == 2;
        }
        bias == 1;
    }


endclass

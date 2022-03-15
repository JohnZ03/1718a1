class conv_core_input_tr;
    rand int signed ima[49];
    rand int signed wei[49];
    rand int signed bias;

    //special case
    constraint c{
        foreach(ima[u]){
            ima[u] == 1;
        }
        foreach(wei[j]){
            wei[j] == 1;
        }
        bias == 1;
    }


endclass

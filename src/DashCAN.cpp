#include <DashCAN.h>


DashCAN::DashCAN(FlexCAN_T4<CAN3, RX_SIZE_256, TX_SIZE_16> *flexcan_bus, u32 node_id, StateVars* state, OLED* oledPtr): flexcan_bus(flexcan_bus), 
            node_id(node_id), increments({false, 0.0, 0.0, 0.0, INCREMENT_P_GAIN, INCREMENT_D_GAIN, INCREMENT_LOW_TARGET, INCREMENT_HIGH_TARGET,0.0}), 
            state(state), oledPtr(oledPtr){

                *state = {false, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0};

            };

void DashCAN::parse_message(const CAN_message_t &msg){
    u32 parsed_node_id = (msg.id >> 5) & 0x3F;
   // Serial.println("message");
    if (parsed_node_id != node_id) {
        return;
    }

    u32 cmd_id = msg.id & 0x1F;
    
    switch (cmd_id) {
        case EG_RPM:
            //add value to scrolling frame circular buffer
            memcpy(&(state->eg_rpm), msg.buf, 4); 
            // Serial.print("\n eg_rpm: ");
            // Serial.print(state->eg_rpm);
            oledPtr->graph_frame.write(state->eg_rpm);
            break;
        case WHEEL_RPM:
            memcpy(&(state->wl_rpm), msg.buf, 4); 

            // Serial.print("\n wl_rpm: ");
            // Serial.print(state->wl_rpm);
            break;
        case P_GAIN:
            memcpy(&(state->p), msg.buf, 4);

            // Serial.print("\n p_gain: ");
            // Serial.print(state->p);
            break;
        case D_GAIN:
            memcpy(&(state->d), msg.buf, 4);
            //Serial.print("\n d_gain: ");
            //Serial.print(state->d);   
            break;   
        case TARGET_RPM:
            memcpy(&(state->targ_rpm), msg.buf,4);
            oledPtr->target_rpms.write(state->targ_rpm);
            break;       
        case LOW_SP_TARG_RPM:
            memcpy(&(state->low_target), msg.buf, 4);

            //Serial.print("\n low_target: ");
            //Serial.print(state->low_target);
            break;
        case HI_SP_TARG_RPM:
            memcpy(&(state->high_target), msg.buf, 4);

            //Serial.print("\n high_target: ");
            //Serial.print(state->high_target);
            break;
        case CLUTCH:
            memcpy(&(state->clutch), msg.buf,1);

            //Serial.print("\n CLUTCH: ");
            //Serial.print(state->clutch);  
            break;
        case ACTUATOR_POS:
            memcpy(&(state->act_pos), msg.buf, 4);

            // Serial.print("\n act_pos: ");
            // Serial.print(state->act_pos); 
            break;
    }
    last_CAN_msg = millis();
}

u8 DashCAN::send_command(u32 cmd_id, bool remote, u8 buf[8]){
    CAN_message_t msg;

    if (cmd_id < 0x00 || 0x11 < cmd_id) {
    return DashCAN::CMD_ERROR_INVALID_COMMAND;
    }

    msg.id = (node_id << 5) | cmd_id;
    msg.len = 8;
    memcpy(&msg.buf, buf, 8);
    msg.flags.remote = remote;

    int write_code = flexcan_bus->write(msg);
    if (write_code == -1) {
    return DashCAN::CMD_ERROR_WRITE_FAILED;
    }
    return DashCAN::CMD_SUCCESS;
}

u8 DashCAN::set_p_gain(float p_gain){
    u8 buf[8] = {0};
    memcpy(buf, &p_gain, 4);
    return send_command(P_GAIN, false, buf);
}

u8 DashCAN::set_d_gain(float d_gain){
    u8 buf[8] = {0};
    memcpy(buf, &d_gain, 4);
    return send_command(D_GAIN, false, buf);
}

u8 DashCAN::set_low_target(float target_rpm){
    u8 buf[8] = {0};
    memcpy(buf, &target_rpm, 4);
    return(send_command(LOW_SP_TARG_RPM, false, buf));
}

u8 DashCAN::set_high_target(float target_rpm){
    u8 buf[8] = {0};
    memcpy(buf, &target_rpm, 4);
    return(send_command(HI_SP_TARG_RPM, false, buf));
}

u8 DashCAN::send_clutch(bool clutch_status){
    u8 buf[8] = {clutch_status};
    return(send_command(CLUTCH, false, buf));
}

u8 DashCAN::send_controller_toggle(bool controller_toggle){
    u8 buf[8] = {controller_toggle};
    return(send_command(CONTROLLER_TOGGLE, false, buf));
}




#ifndef SHARED_H
#define SHARED_H

typedef int PacketType;
const PacketType LOGIN=0;
const PacketType SIGN_UP=1;
const PacketType GET_USERS=2;
const PacketType ACCEPT_USER=3;
const PacketType WORK_SUBMISSION=4;
const PacketType GET_NOTIFICATION=5;
const PacketType GET_DEADLINE=6;
const PacketType SET_DEADLINE=7;
const PacketType SET_NOTIFICATIONS=8;
const PacketType SET_COMMENT=9;
const PacketType SET_REVIEW=10;
const PacketType GET_REVIEWS=11;
const PacketType GET_NEWS=12;
const PacketType SET_NEWS=13;

#endif

//
// Created by root on 20.06.15.
//

#ifndef LB4_CLIENT_HPP
#define LB4_CLIENT_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include <unistd.h>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "../headers/serialization.hpp"
#include "../headers/types.hpp"
#include<string.h>    //strlen
#include<string>  //string
#include "../server/task.hpp"


#define BUF_SIZE 10000


#endif //LB4_CLIENT_HPP


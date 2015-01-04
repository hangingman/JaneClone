#!/bin/bash

echo "#include <string>" > keys/apikeys.hpp
echo static const std::string AP_COMSUMER_KEY=\"$CONSUMER_KEY\"\; >> keys/apikeys.hpp
echo static const std::string AP_COMSUMER_SECRET=\"$CONSUMER_SECRET\"\; >> keys/apikeys.hpp


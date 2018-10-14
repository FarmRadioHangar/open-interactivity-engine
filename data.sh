#!/bin/bash

# Add some languages
curl -XPOST http://localhost:9080/languages -d '{"name":"English","tag":"en"}'
curl -XPOST http://localhost:9080/languages -d '{"name":"Swahili","tag":"sw"}'
curl -XPOST http://localhost:9080/languages -d '{"name":"French","tag":"fr"}'

# Create a listener audience
curl -XPOST http://localhost:9080/audience -d '{"name":"Sun Ra","attributes":{"farmer":true}}'
curl -XPOST http://localhost:9080/audience -d '{"name":"Grace Jones","attributes":{}}'
curl -XPOST http://localhost:9080/audience -d '{"name":"Uri Geller","attributes":{"farmer":true}}'
curl -XPOST http://localhost:9080/audience -d '{"name":"Zlatan","attributes":{}}'
curl -XPOST http://localhost:9080/audience -d '{"name":"Geordi La Forge","attributes":{}}'

# Countries
curl -XPOST http://localhost:9080/countries -d '{"name":"Canada","dialingCode":1,"alpha2Code":"ca"}'
curl -XPOST http://localhost:9080/countries -d '{"name":"Tanzania","dialingCode":255,"alpha2Code":"tz"}'
curl -XPOST http://localhost:9080/countries -d '{"name":"Uganda","dialingCode":256,"alpha2Code":"ug"}'

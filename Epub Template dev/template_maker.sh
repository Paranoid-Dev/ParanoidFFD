#!/bin/bash

cd pre-ParanoidFFD_Template
zip -X -0 pre-ParanoidFFD_Template mimetype
zip -9 -r pre-ParanoidFFD_Template META-INF
zip -9 pre-ParanoidFFD_Template template.css
cd ..
mv pre-ParanoidFFD_Template/pre-ParanoidFFD_Template.zip pre-ParanoidFFD_Template.epub
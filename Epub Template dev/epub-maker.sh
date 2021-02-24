#!/bin/bash

cp pre-ParanoidFFD_Template.epub post-ParanoidFFD_Template.zip
zip -9 post-ParanoidFFD_Template.zip content.opf toc.ncx *.xhtml
mv post-ParanoidFFD_Template.zip ParanoidFFD_Template.epub

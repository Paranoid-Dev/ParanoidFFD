#!/bin/bash
xxd -i pre-ParanoidFFD_Template.epub > pre_ParanoidFFD_Template_epub_char
sed -i "s/\[\]/\[`tail -1 pre_ParanoidFFD_Template_epub_char | awk '{print $NF}' | sed -e 's/;//g'`\]/g" pre_ParanoidFFD_Template_epub_char
sed -i '$ d' pre_ParanoidFFD_Template_epub_char
perl -p -i -e 's/\R//g;' pre_ParanoidFFD_Template_epub_char
sed -i -e "s/  / /g" pre_ParanoidFFD_Template_epub_char
sed -i -e "s/{ /{/g" pre_ParanoidFFD_Template_epub_char

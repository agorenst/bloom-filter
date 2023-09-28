#/bin/bash
/usr/lib/noweb/markup $1 |
sed "/^@use /s/_/\\_/g;/^@defn /s/_/\\_/g" |
python3 MyNoweb.py $1 |
/usr/lib/noweb/totex -noindex -delay

git branch -D gh-pages 
git checkout -b gh-pages
./generate_doc.sh
git add index.html
git add search.json
git add xml
git commit -m "DOC: Generated"

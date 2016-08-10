git branch -D gh-pages 
git checkout -b gh-pages

doxygen Doxyfile

git add -f html/
git commit -m "DOC: Generated"

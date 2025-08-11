build()
{
    echo make -f make/Makefile $@
    make -f make/Makefile $@
}
suffix()
{
    suff=${@: -1}
    words=("${@:1:$((($#)-1))}")

    res=""
    for word in $words;do
        res="$res$word$suff "
    done
    echo "$res"
}
compile()
{
    if [ "$#" -eq 0 ];then
        makeproj compile_all
    else
        makeproj $(suffix $1 _compile) ${@:2}
    fi
}
link()
{
    if [ "$#" -eq 0 ];then
        makeproj link_all
    else
        makeproj $(suffix $1 _link) ${@:2}
    fi
}
run()
{
    makeproj $(suffix $1 _run) ARGS="\"${*:2}\""
}
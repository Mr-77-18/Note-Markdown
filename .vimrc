set number
imap jk <Esc>
set hlsearch
nmap  lq :FSHere<CR>
let &t_ut=''
"tags------------------------------------------------------
"set tags+=/home/liusenhong/liu/UEFI/tags
"set tags+=/home/liusenhong/liu/UEFI/edk2-platforms/Platform/Loongson/tags
"set tags+=/home/liusenhong/liu/linux-2.4.0/tags
"set tags+=/home/liusenhong/liu/ROS/ros2/tags
"set tags+=/opt/ros/tags
"set tags+=/home/liusenhong/liu/Openmp/EPCC-OpenMP-micro-benchmarks/tags
"set tags+=/home/liusenhong/liu/ROS/ros2/rmw_fastrtps/tags
set tags+=/home/liusenhong/liu/c++/MyTinySTL/MyTinySTL/tags
set tags+=/home/liusenhong/liu/Openmp/EPCC-OpenMP-micro-benchmarks/tags
set tags+=/home/liusenhong/liu/Openmp/NPB3.4.2/NPB3.4-OMP/tags






"------------------------------------------------------------
set nofoldenable
set foldmethod=manual
augroup remember_folds
	autocmd!
	au BufWinLeave ?* mkview 1
	au BufWinEnter ?* silent! loadview 1
augroup END




"vim rainbow 括号不同颜色------------------------------------
let g:rainbow_active = 1
"NERDTree目录树--------------------------------------------
nnoremap <C-n> :NERDTree<CR>




"markdownpreview------------------------------------------
nmap <C-s> <Plug>MarkdownPreview
nmap <M-s> <Plug>MarkdownPreviewStop
nmap <C-p> <Plug>MarkdownPreviewToggle

"let g:mkdp_open_to_the_world = 1
"let g:mkdp_open_ip = '192.168.80.135'
"let g:mkdp_port = 8080
"function! g:Open_browser(url)
"	silent exe '!lemonade  open'a:url
"		endfunction
"let g:mkdp_browserfunc = 'g:Open_browser'
"let g:mkdp_open_ip = '172.31.225.97'
let g:mkdp_open_to_the_world = 1
let g:mkdp_open_ip = '192.168.80.135' " change to you vps or vm ip
let g:mkdp_port = 8080
function! g:EchoUrl(url)
		":!lemonade --host='193.168.80.1' --port=2489 open 'http://192.168.80.135:8080/page/1'
		:!lemonade.sh
endfunction
    let g:mkdp_browserfunc = 'g:EchoUrl'


"quick control of markdown
"autocmd Filetype markdown inoremap ,b **** <++><Esc>F*hi
"autocmd Filetype markdown inoremap ,f <Esc>/<++><CR>:nohlsearch<CR>c4l
"autocmd Filetype markdown map <leader>w yiWi[<esc>Ea](<esc>pa)
autocmd Filetype markdown inoremap ,f <Esc>/<++><CR>:nohlsearch<CR>c4l
autocmd Filetype markdown inoremap ,n ---<Enter><Enter>
autocmd Filetype markdown inoremap ,b **** <++><Esc>F*hi
autocmd Filetype markdown inoremap ,s ~~~~ <++><Esc>F~hi
autocmd Filetype markdown inoremap ,i ** <++><Esc>F*i
autocmd Filetype markdown inoremap ,d `` <++><Esc>F`i
autocmd Filetype markdown inoremap ,c ```<Enter><++><Enter>`````<Enter><Enter><++><Esc>4kA
autocmd Filetype markdown inoremap ,h ====<Space><++><Esc>F=hi
autocmd Filetype markdown inoremap ,p ![](<++>) <++><Esc>F[a
autocmd Filetype markdown inoremap ,a [](<++>) <++><Esc>F[a
autocmd Filetype markdown inoremap ,1 #<Space><Enter><++><Esc>kA
autocmd Filetype markdown inoremap ,2 ##<Space><Enter><++><Esc>kA
autocmd Filetype markdown inoremap ,3 ###<Space><Enter><++><Esc>kA
autocmd Filetype markdown inoremap ,4 ####<Space><Enter><++><Esc>kA
autocmd Filetype markdown inoremap ,l --------<Enter>

"vundle plugin----------------------------------------------
filetype off
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()
Plugin 'VundleVim/Vundle.vim'
Plugin 'derekwyatt/vim-fswitch'
Plugin 'vim-airline/vim-airline'
Plugin 'sheerun/vim-polyglot'
Plugin 'luochen1990/rainbow'
Plugin 'preservim/nerdtree'
Plugin 'dense-analysis/ale'
Plugin 'jiangmiao/auto-pairs'
Plugin 'iamcco/markdown-preview.nvim'
call vundle#end()
filetype plugin indent on
"other section-----------------------------------------------
"filetype on
"set laststatus=2
"" 显示光标当前位置
" set ruler
"" " 开启行号显示
" set number
"" " 高亮显示当前行/列
" set cursorline
" set cursorcolumn
"" " 高亮显示搜索结果
" set hlsearch
""
"
"" 禁止光标闪烁
"set gcr=a:block-blinkon0
"" " 禁止显示滚动条
" set guioptions-=l
" set guioptions-=L
" set guioptions-=r
" set guioptions-=R
"" " 禁止显示菜单和工具条
" set guioptions-=m
" set guioptions-=T
"
"
"
""set color
""set t_Co=256
""set background=dark
set background=light "//支持dark / light两种色彩, ternimal

"cpp quick control
autocmd Filetype cpp inoremap ,w (<++>){<CR>}<Esc>/<++><CR>:nohlsearch<CR>c4l

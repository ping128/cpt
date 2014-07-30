;; setting by ping128


;; auto-insert-mode setting
(require 'autoinsert) 
(auto-insert-mode) 
(setq auto-insert-directory "~/.emacs.d/mytemplates") 
(setq auto-insert-query nil) 
(define-auto-insert "\.cpp" "normal.cpp")
;(define-auto-insert "\.cpp" "cases.cpp")

;; line number
(global-linum-mode t)

;; 4 spaces indentation
(setq basic-offset 4)
(setq c-basic-offset 4)

;; auto-complete-1.3.1
(add-to-list 'load-path "~/.emacs.d/auto-complete-1.3.1")
(require 'auto-complete-config)
(add-to-list 'ac-dictionary-directories "~/.emacs.d/auto-complete-1.3.1/ac-dict")
(ac-config-default)

;; Use spaces instead of tabs
(setq-default indent-tabs-mode nil)
(setq-default tab-width 4)

;; imenu
(add-hook 'prog-mode-hook 'imenu-add-menubar-index)
(setq imenu-auto-rescan t)

(custom-set-variables
 ;; custom-set-variables was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 '(blink-cursor-mode nil)
 '(column-number-mode t)
 '(custom-safe-themes
   (quote
    ("fc5fcb6f1f1c1bc01305694c59a1a861b008c534cae8d0e48e4d5e81ad718bc6" "1e7e097ec8cb1f8c3a912d7e1e0331caeed49fef6cff220be63bd2a6ba4cc365" "8aebf25556399b58091e533e455dd50a6a9cba958cc4ebb0aab175863c25b9a4" "e16a771a13a202ee6e276d06098bc77f008b73bbac4d526f160faa2d76c1dd0e" "8b03a79bf4aa6f61e6ab2e8d0d74904c61eca17e35b5e25656d20b3be23c2b74" "2ab91cb47985c4923e10f6d8fa9d5d298e047feb36ba1c43c79cdb88cde9bb95" "484fffbe5ae378380c6a194278e0adc2d2c67310d6ced8de9c8b54f997567452" "c2cfe2f1440d9ef4bfd3ef4cf15bfe35ff40e6d431264b1e24af64f145cffb11" default)))
 '(inhibit-startup-screen t)
 '(show-paren-mode t)
 '(tool-bar-mode nil))
(custom-set-faces
 ;; custom-set-faces was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 )

;; Set font
 (add-to-list 'default-frame-alist
              '(font . "Consolas-14"))
;'(font . "DejaVu Sans Mono-13"))

;; Helm plugin
(add-to-list 'load-path "/Users/ping128/.emacs.d/helm")
(require 'helm-config)

;; (global-set-key (kbd "C-x C-f") 'helm-find-files)
(global-set-key (kbd "C-x C-b") 'helm-mini)

;; Theme
(add-to-list 'custom-theme-load-path "~/.emacs.d/themes/")
(load-theme 'zenburn t)

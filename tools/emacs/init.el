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
(load-theme 'whiteboard t)
;(load-theme 'zenburn t)

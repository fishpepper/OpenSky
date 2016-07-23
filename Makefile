#select target for build, available: VD5M, D4RII
TARGET ?= D4RII

ifeq ($(TARGET),D4RII)
  include board/d4rii/Makefile.board
else 
  ifeq ($(TARGET),VD5M)
    include board/vd5m/Makefile.board
  else
    ERROR
  endif
endif

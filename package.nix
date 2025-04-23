{
  lib,
  stdenv,
  fetchFromGitHub,
}:

stdenv.mkDerivation rec {
  pname = "judgel";
  version = "2.0.0";

  src = fetchFromGitHub {
    owner = "udontur";
    repo = "judgel";
    tag = "v${version}";
    hash = "sha256-Sr1V6wN+4IDXh3o/zy0hPYdsRfLhyKwlYzu407KMoQA=";
  };

  installPhase = ''
    runHook preInstall
    install -Dm755 ./judgel $out/bin/judgel
    runHook postInstall
  '';

  meta = {
    description = "Simple local C++ CLI judge";
    homepage = "https://github.com/udontur/judgel";
    license = lib.licenses.mit;
    maintainers = with lib.maintainers; [ udontur ];
    mainProgram = "judgel";
    platforms = lib.intersectLists lib.platforms.linux lib.platforms.x86;
  };
}


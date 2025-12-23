[frum](https://github.com/TaKO8Ki/frum) is a Ruby version manager that makes it easy[^definition] to install multiple versions of Ruby.

[^definition]: For some definition of "easy".

Install prerequisites as documented here:

- https://github.com/TaKO8Ki/frum/issues/113#issuecomment-2483940932
- https://github.com/TaKO8Ki/frum/issues/113#issuecomment-1698148674


```
brew install autoconf automake frum libtool libyaml openssl@1.1 openssl@3 pkg-config readline zlib
```

Build versions 3.1 and above using OpenSSL v3:

```
export RUBY_CONFIGURE_OPTS="--with-openssl-dir=$(brew --prefix openssl@3) --with-libyaml-dir=$(brew --prefix libyaml) --with-readline-dir=$(brew --prefix readline)"

frum install 3.4.8
frum install 3.3.10
frum install 3.2.9
frum install 3.1.7
```

Build versions below 3.1 using OpenSSL v1.1 (many of these fail on macOS Sequoia due to Clang being strict):

```
export RUBY_CONFIGURE_OPTS="--with-openssl-dir=$(brew --prefix openssl@1.1) --with-libyaml-dir=$(brew --prefix libyaml) --with-readline-dir=$(brew --prefix readline)"

frum install 3.0.7 # fails to install
frum install 2.7.8
frum install 2.6.10 # fails to install with Clang
frum install 2.5.9 # fails to install with Clang
frum install 2.4.10 # fails to install with Clang
frum install 2.3.8 # fails to install with Clang
frum install 2.2.10 # fails to install with Clang
```

Test with different Ruby versions:

```
eval "$(frum init)"

for VERSION in 3.4.8 3.3.10 3.2.9 3.1.7 2.7.8; do
    echo "Starting test with $VERSION"
    frum local $VERSION
    rm -f Gemfile.lock
    bundle
    bundle exec rake clean
    bundle exec rake make
    bundle exec rake spec
    echo "Finished test with $VERSION"
done
```

# Releasing

Update `lib/wikitext/version.rb` and `doc/RELEASE-NOTES`, then:

```
git commit -p -m 'chore: prepare for 4.9 release'
git tag -s 4.9 -m '4.9 release'
git push --follow-tags

bundle exec rake build
bundle exec rake push

git worktree add gh-pages origin/gh-pages # First time only.

bundle exec rake yard
cd gh-pages
git commit -p -m 'docs: run `bundle exec rake yard` for 4.9 release'
git push
```

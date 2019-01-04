# -*- mode: ruby -*-
# vi: set ft=ruby :

Vagrant.configure("2") do |config|
  config.vm.box = "ubuntu/bionic64"

  # Disable automatic box update checking. If you disable this, then
  # boxes will only be checked for updates when the user runs
  # `vagrant box outdated`. This is not recommended.
  # config.vm.box_check_update = false

  # Provider-specific configuration so you can fine-tune various
  # backing providers for Vagrant. These expose provider-specific options.
  # Example for VirtualBox:
  #
  config.vm.provider "virtualbox" do |vb|
  #   # Customize the amount of memory on the VM:
  #   vb.memory = "1024"
    # Enable USB
    vb.customize ['modifyvm', :id, '--usb', 'on']
    # Please use your esp device id, run `VBoxManage.exe list usbhost` list devices
    vb.customize ['usbfilter', 'add', '0', '--target', :id, '--name', 'ESP', '--vendorid', '0x10C4', '--productid', '0xEA60']
    # Disable log
    vb.customize ['modifyvm', :id, '--uartmode1', 'disconnected']
  end

  # Enable provisioning with a shell script. Additional provisioners such as
  # Puppet, Chef, Ansible, Salt, and Docker are also available. Please see the
  # documentation for more information about their specific syntax and use.
  config.vm.provision "shell", inline: <<-SHELL
    apt-get -qq update && apt-get -qq upgrade -y
    apt-get -qq install -y linux-image-extra-virtual
    apt-get -qq install -y gcc git wget make libncurses-dev flex bison gperf python python-pip python-setuptools python-serial python-cryptography python-future python-pyparsing
    if [ ! -d /opt/local/espressif/ ]; then
      mkdir -p /opt/local/espressif/
    fi
    if [ ! -d /opt/local/espressif/xtensa-esp32-elf/ ]; then
      wget -qO- https://dl.espressif.com/dl/xtensa-esp32-elf-linux64-1.22.0-80-g6c4433a-5.2.0.tar.gz | tar xz -C /opt/local/espressif/
    fi
    if [ ! -d /opt/local/espressif/esp-idf/ ]; then
      git clone -q --recursive https://github.com/espressif/esp-idf.git /opt/local/espressif/esp-idf
    else
      cd /opt/local/espressif/esp-idf && git pull -q && git submodule --quiet update --init --recursive
    fi
    chown -R vagrant: /opt/local/espressif/
    usermod -a -G dialout vagrant
  SHELL
  config.vm.provision 'shell', privileged: false, inline: <<-SHELL
    python -m pip install -q --user -r /opt/local/espressif/esp-idf/requirements.txt
    grep -q 'IDF_PATH' /home/vagrant/.profile || echo "export IDF_PATH=\"/opt/local/espressif/esp-idf\"\nexport PATH=\"/opt/local/espressif/xtensa-esp32-elf/bin:${PATH}\"\ncd /vagrant" >> /home/vagrant/.profile
  SHELL
end

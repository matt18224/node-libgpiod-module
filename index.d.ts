/**
 * Chip instances represent GPIO chips (each with a fixed number of
 * GPIO lines) on the Linux host. The number of GPIO chips depends on
 * the hardware available.
 */
export declare class Chip {
    /**
     * Constructs a new Chip instance by its chip number.
     *
     * @param chipNumber the number of the GPIO chip
     * @throws error if the chip cannot be opened
     */
    constructor(chipNumber: number);

    /**
     * Constructs a new Chip instance by its device name or full device path.
     *
     * @param deviceNameOrPath the device name or full device path of the GPIO chip
     * @throws error if the chip cannot be opened
     */
    constructor(deviceNameOrPath: string);

    /**
     * Returns the number of GPIO lines available (reserved or not) on the GPIO chip.
     */
    getNumberOfLines(): number

    /**
     * Returns the GPIO chip name as represented by the kernel.
     */
    getChipName(): string

    /**
     * Returns the GPIO chip label as represented by the kernel.
     */
    getChipLabel(): string

    createRequest(): RequestBuilder;
}

/**
 * Returns true if the libgpiod functionality is available in
 * the current execution environment, otherwise false (for example
 * in non-Linux development environments).
 */
export declare function available(): boolean;

export declare function usleep(microseconds: number): void;

export declare function readBit(lineRequest: LineRequest, pdSckPinOffset: number, doutPinOffset: number): number;

export declare function readLong(lineRequest: LineRequest, pdSckPinOffset: number, doutPinOffset: number, gain: 1 | 2 | 3)

type Direction = "INPUT" | "OUTPUT" | "AS_IS";

export declare class LineSettings {
    constructor() {

    }

    getDirection(): Direction;

    setDirection(direction: Direction): LineSettings;

    reset(): void;

    setOutputValue(value: 0 | 1): LineSettings;

    getOutputValue(): 0 | 1;
}

export declare class LineRequest {
    getValue(offset: number): 0 | 1;

    setValue(offset: number, value: 0 | 1): void;

    release(): void;

    reconfigureLines(config: LineConfig): void;
}

export declare class RequestBuilder {
    addLineSettings(offset: number, lineSettings: LineSettings): RequestBuilder;

    setLineConfig(lineConfig: LineConfig): RequestBuilder;

    doRequest(): LineRequest;
}

export declare class LineConfig {
    reset(): void;

    addLineSetting(lineSettings: LineSettings): LineConfig

    static createFromLiteral(mapping: LineConfigMapping): LineConfig
}

export declare type LineConfigMapping = {
    [k: number]: LineSettings
}
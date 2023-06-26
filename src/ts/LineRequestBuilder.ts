export type Offset = number;

export type LineDirection = 'INPUT' | 'OUTPUT' | 'AS_IS'

export interface LineConfig {
    settings: Record<Offset, LineSettings>
}

export interface LineSettings {
    direction: LineDirection
    outputValue?: 0 | 1
}

export interface RequestConfig {
    consumer?: string;
}

export interface LineRequestBuilder {
    lineConfig: LineConfig,

}